#include "xml2objectwidget.h"

Xml2ObjectWidget::Xml2ObjectWidget(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
	setupUi(this);

	code_hl=new SyntaxHighlighter(code_txt);
	code_hl->loadConfiguration(GlobalAttributes::XMLHighlightConfPath);

	connect(close_btn, SIGNAL(clicked(void)), this, SLOT(close(void)));
	connect(clear_btn, SIGNAL(clicked(void)), this, SLOT(clearSource(void)));
	connect(load_btn, SIGNAL(clicked(void)), this, SLOT(loadXML(void)));
	connect(generate_btn, SIGNAL(clicked(void)), this, SLOT(generateObject(void)));
}

void Xml2ObjectWidget::clearSource(void)
{
	code_txt->setPlainText(QString("<dbmodel>\n\n</dbmodel>"));
}

void Xml2ObjectWidget::loadXML(void)
{
	QFileDialog file_dlg;

	file_dlg.setNameFilter(trUtf8("Database model (*.dbm);;XML file (*.xml);;All files (*.*)"));
	file_dlg.setWindowTitle(trUtf8("Load file"));
	file_dlg.setFileMode(QFileDialog::ExistingFiles);
	file_dlg.setAcceptMode(QFileDialog::AcceptOpen);

	if(file_dlg.exec()==QFileDialog::Accepted)
	{
		QFile f(file_dlg.selectedFiles().at(0));

		f.open(QFile::ReadOnly);

		if(!f.isOpen())
			throw Exception(Exception::getErrorMessage(ErrorCode::FileDirectoryNotAccessed).arg(file_dlg.selectedFiles().at(0)),
											ErrorCode::FileDirectoryNotAccessed,__PRETTY_FUNCTION__,__FILE__,__LINE__);


		code_txt->setPlainText(QString::fromUtf8(f.readAll()));
		f.close();
	}
}

void Xml2ObjectWidget::show(DatabaseModel *model, OperationList *op_list)
{
	clearSource();
	this->setEnabled(model!=nullptr && op_list!=nullptr);
	this->op_list=op_list;
	this->model=model;
	QDialog::show();
}

void Xml2ObjectWidget::generateObject(void)
{
	try
	{
		BaseObject *object=nullptr;
		ObjectType obj_type;
		QString elem_name;
		XmlParser *xmlparser=model->getXMLParser();

		if(!op_list->isOperationChainStarted())
			op_list->startOperationChain();

		xmlparser->restartParser();
		xmlparser->loadXMLBuffer(code_txt->toPlainText().toUtf8());

		if(xmlparser->accessElement(XmlParser::ChildElement))
		{
			do
			{
				if(xmlparser->getElementType()==XML_ELEMENT_NODE)
				{
					elem_name=xmlparser->getElementName();
					obj_type=BaseObject::getObjectType(elem_name);

					xmlparser->savePosition();

					object=model->createObject(obj_type);

					if(object && !dynamic_cast<TableObject *>(object) &&
							obj_type!=ObjectType::Relationship && obj_type!=ObjectType::BaseRelationship)
					{
						model->addObject(object);
						op_list->registerObject(object, Operation::ObjectCreated, -1, model);
					}

					xmlparser->restorePosition();
				}
			}
			while(xmlparser->accessElement(xmlparser->NextElement));
		}

		op_list->finishOperationChain();
	}
	catch(Exception &e)
	{
		if(op_list->isOperationChainStarted())
			op_list->finishOperationChain();

		op_list->undoOperation();
		op_list->removeLastOperation();
		throw Exception(e.getErrorMessage(), e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

