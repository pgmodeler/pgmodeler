#include "xml2objectwidget.h"

Xml2ObjectWidget::Xml2ObjectWidget(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
	setupUi(this);

	code_hl=new SyntaxHighlighter(code_txt, true);
	code_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
															GlobalAttributes::DIR_SEPARATOR +
															GlobalAttributes::XML_HIGHLIGHT_CONF +
															GlobalAttributes::CONFIGURATION_EXT);

	connect(close_btn, SIGNAL(clicked(void)), this, SLOT(close(void)));
	connect(clear_btn, SIGNAL(clicked(void)), this, SLOT(clearSource(void)));
	connect(load_btn, SIGNAL(clicked(void)), this, SLOT(loadXML(void)));
	connect(generate_btn, SIGNAL(clicked(void)), this, SLOT(generateObject(void)));
}

void Xml2ObjectWidget::clearSource(void)
{
	code_txt->setText("<dbmodel>\n\n</dbmodel>");
}

void Xml2ObjectWidget::loadXML(void)
{
	QFileDialog file_dlg;

	file_dlg.setFilter(trUtf8("Database model (*.dbm);;XML file (*.xml);;All files (*.*)"));
	file_dlg.setWindowIcon(QPixmap(QString(":/icones/icones/pgsqlModeler48x48.png")));
	file_dlg.setWindowTitle(trUtf8("Load file"));
	file_dlg.setFileMode(QFileDialog::ExistingFiles);
	file_dlg.setAcceptMode(QFileDialog::AcceptOpen);

	if(file_dlg.exec()==QFileDialog::Accepted)
	{
		QFile f(file_dlg.selectedFiles().at(0));

		f.open(QFile::ReadOnly);

		if(!f.isOpen())
			throw Exception(Exception::getErrorMessage(ERR_FILE_DIR_NOT_ACCESSED).arg(file_dlg.selectedFiles().at(0)),
											ERR_FILE_DIR_NOT_ACCESSED,__PRETTY_FUNCTION__,__FILE__,__LINE__);


		code_txt->setText(QString::fromUtf8(f.readAll()));
		f.close();
	}
}

void Xml2ObjectWidget::show(DatabaseModel *model, OperationList *op_list)
{
	clearSource();
	this->setEnabled(model!=NULL && op_list!=NULL);
	this->op_list=op_list;
	this->model=model;
	QDialog::show();
}

void Xml2ObjectWidget::generateObject(void)
{
	try
	{
		BaseObject *object=NULL;
		ObjectType obj_type;
		QString elem_name;

		if(!op_list->isOperationChainStarted())
			op_list->startOperationChain();

		XMLParser::restartParser();
		XMLParser::loadXMLBuffer(code_txt->toPlainText().toUtf8());

		if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
		{
			do
			{
				if(XMLParser::getElementType()==XML_ELEMENT_NODE)
				{
					elem_name=XMLParser::getElementName();
					obj_type=model->getObjectType(elem_name);

					XMLParser::savePosition();

					object=model->createObject(obj_type);

					if(object && !dynamic_cast<TableObject *>(object) &&
						 obj_type!=OBJ_RELATIONSHIP && obj_type!=BASE_RELATIONSHIP)
					{
						model->addObject(object);
						op_list->registerObject(object, Operation::OBJECT_CREATED, -1, model);
					}

					XMLParser::restorePosition();
				}
			}
			while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
		}

		op_list->finishOperationChain();
	}
	catch(Exception &e)
	{
		if(op_list->isOperationChainStarted())
			op_list->finishOperationChain();

		op_list->undoOperation();
		op_list->removeLastOperation();
		throw Exception(e.getErrorMessage(), e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

