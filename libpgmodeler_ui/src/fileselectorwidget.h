#ifndef FILESELECTORWIDGET_H
#define FILESELECTORWIDGET_H

#include <QLineEdit>
#include <QToolButton>
#include <QFileDialog>
#include <QTimer>

struct FileSelector
{
	enum Stream{
		In,
		Out
	};
	enum FileType{
		Directory,
		Sql,
		DatabaseModel,
		Csv,
		Cli,
		SourceCodeEditor,
		ObjectMetadata,
		Svg,
		Png,
		Html
	};

	Stream stream;
	FileType filetype;
};

class FileSelectorWidget : public QWidget
{
private:
	Q_OBJECT

	QLineEdit *file_edt;
	QToolButton *select_file_tb;

	FileSelector *file_selector;

	QTimer *timer;

public:

	FileSelectorWidget(QWidget *placeholder, QWidget *parent, FileSelector *fsel);

	QString text();

private slots:
	void handleTimeout();
public slots:
	void reloadFileSelector(FileSelector *fsel);
	void setText(QString path);
	void setupFileDialog();
	void handleTextChanged(const QString path);
signals:
	void s_fileSelected(const QString path);
};

#endif // FILESELECTORWIDGET_H
