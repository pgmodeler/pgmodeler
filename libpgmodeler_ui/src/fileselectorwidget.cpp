#include <QHBoxLayout>
#include "fileselectorwidget.h"
#include "exception.h"

FileSelectorWidget::FileSelectorWidget(
		QWidget *placeholder, QWidget *parent, FileSelector *fsel) : QWidget(parent)
{
	if(!placeholder)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	file_selector=fsel;

	auto horizontalLayout = new QHBoxLayout(placeholder);
	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
	horizontalLayout->setMargin(0);
	horizontalLayout->setContentsMargins(0,0,0,0);

	file_edt = new QLineEdit(this);
	file_edt->setObjectName(QString::fromUtf8("file_edt"));
	file_edt->setEnabled(true);
	file_edt->setDragEnabled(false);
	file_edt->setClearButtonEnabled(true);

	horizontalLayout->addWidget(file_edt);

	select_file_tb = new QToolButton(this);
	select_file_tb->setObjectName(QString::fromUtf8("select_file_tb"));
	select_file_tb->setEnabled(true);
	select_file_tb->setMinimumSize(QSize(0, 0));
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/icones/icones/abrir.png"), QSize(), QIcon::Normal, QIcon::Off);
	select_file_tb->setIcon(icon1);
	select_file_tb->setIconSize(QSize(22, 22));

	horizontalLayout->addWidget(select_file_tb);

	placeholder->setLayout(horizontalLayout);

	//Signalling init
	connect(select_file_tb, SIGNAL(clicked()), this, SLOT(setupFileDialog()));
	connect(file_edt, SIGNAL(textChanged(QString)),
			this, SLOT(handleTextChanged(QString)));

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));

}

void FileSelectorWidget::setupFileDialog()
{

	QFileDialog file_dlg;

	file_dlg.setLabelText(QFileDialog::Accept, "Set");
	file_dlg.setLabelText(QFileDialog::Reject, "Cancel");
	file_dlg.setModal(false);

	switch(file_selector->stream)
	{
		case(FileSelector::In):
		{
			file_dlg.setWindowTitle("Open :");
			file_dlg.setAcceptMode(QFileDialog::AcceptOpen);
			file_dlg.setFileMode(QFileDialog::ExistingFile);
			file_dlg.setOption(QFileDialog::DontConfirmOverwrite);
		}
			break;
		case(FileSelector::Out):
		{
			file_dlg.setWindowTitle("Save :");
			file_dlg.setAcceptMode(QFileDialog::AcceptSave);
			file_dlg.setFileMode(QFileDialog::AnyFile);
		}
	};

	switch(file_selector->filetype)
	{
		case(FileSelector::Directory):
			file_dlg.setDefaultSuffix("");
			file_dlg.setMimeTypeFilters({"application/octet-stream"});
			file_dlg.setOption(QFileDialog::ShowDirsOnly);
			file_dlg.setFileMode(QFileDialog::DirectoryOnly);
		break;
		case(FileSelector::Sql):
			file_dlg.setDefaultSuffix("sql");
			file_dlg.setMimeTypeFilters({"application/sql","application/octet-stream"});
		break;
		case(FileSelector::DatabaseModel):
			file_dlg.setDefaultSuffix("dbm");
			file_dlg.setNameFilter("PgModeler-db-model file (*.dbm);;All files (*.*)");
		break;
		case(FileSelector::Csv):
			file_dlg.setDefaultSuffix("csv");
			file_dlg.setMimeTypeFilters({"text/csv","application/octet-stream"});
		break;
		case(FileSelector::Cli):
		case(FileSelector::SourceCodeEditor):
#ifdef Q_OS_WIN
			file_dlg.setDefaultSuffix("exe");
			file_dlg.setMimeTypeFilters{"application/x-ms-dos-executable","application/octet-stream"};
#else
			file_dlg.setDefaultSuffix("");
			file_dlg.setMimeTypeFilters({"","application/octet-stream"});
#endif
		break;
		case(FileSelector::ObjectMetadata):
			file_dlg.setDefaultSuffix("omf");
			file_dlg.setNameFilter("Objects metadata file (*.omf);;All files (*.*)");
		break;
		case(FileSelector::Svg):
			file_dlg.setDefaultSuffix("svg");
			file_dlg.setMimeTypeFilters({"image/svg+xml","application/octet-stream"});
		break;
		case(FileSelector::Png):
			file_dlg.setDefaultSuffix("png");
			file_dlg.setMimeTypeFilters({"image/png","application/octet-stream"});
		break;
		case(FileSelector::Html):
			file_dlg.setDefaultSuffix("html");
			file_dlg.setMimeTypeFilters({"text/html","application/octet-stream"});
	}

	connect(&file_dlg, SIGNAL(fileSelected(QString)), file_edt, SLOT(setText(QString)));
	file_dlg.exec();
}

void FileSelectorWidget::handleTimeout()
{
	if(this->isEnabled() && file_edt->text()!="")
		handleTextChanged(file_edt->text());
	else
		timer->stop();
}


void FileSelectorWidget::handleTextChanged(const QString text)
{
	//todo Imagining (|=widget border) : |/this/is/quite/a/very/long|/path/to/a/file turning into :
	// |/this/is[...]/path/to/a/file|
	// lineEdit->text().count() - qlineedit.width/ QFontMetrics::averageCharWidth > 0 ?
	// would have to dig into qt sources around QString::left/rightJustified for border & clear button

	if(text==nullptr)
	{
		emit s_fileSelected(nullptr);
		return;
	}

	//todo Would be better to run an xterm/shell than all this (NIH-ing)?
	QPalette pal=file_edt->palette();

	QFileInfo fi(text);
	if(!fi.exists())
	{
		if(file_selector->stream==FileSelector::In || file_selector->filetype==FileSelector::Directory)
			pal.setColor(QPalette::Text, Qt::red);
		else
			pal.setColor(QPalette::Text, Qt::darkGreen);
	}
	else
	{
		if(file_selector->stream==FileSelector::In || file_selector->filetype==FileSelector::Directory)
			pal.setColor(QPalette::Text, Qt::black);
		else
			pal.setColor(QPalette::Text, QColor(240,121,23));
	}

	file_edt->setPalette(pal);

	//poll disk for changes every 3s, which qtcreator does not currently.
	if(!timer->isActive())
		timer->start(3000);

	if(file_selector->stream==FileSelector::Out || fi.exists())
	{
		emit s_fileSelected(text);
		//todo sender not dlg
//		if(fi.isFile())
//		{
//			file_selector->dialog_captions[0]=fi.absoluteDir().path();
//			file_selector->dialog_captions[1]=fi.fileName();
//		}
//		else if(fi.isDir())
//		{
//			file_selector->dialog_captions[0]=fi.absoluteFilePath();
//			file_selector->dialog_captions[1]="";
//		}
	}
}

QString FileSelectorWidget::text()
{
	return file_edt->text();
}

void FileSelectorWidget::setText(QString path)
{
	file_edt->setText(path);
}

void FileSelectorWidget::reloadFileSelector(FileSelector *fsel)
{
	file_selector->stream=fsel->stream;
	file_selector->filetype=fsel->filetype;
	//todo qsettings
	handleTextChanged(file_edt->text());
}
