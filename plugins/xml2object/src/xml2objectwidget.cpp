#include "xml2objectwidget.h"

Xml2ObjectWidget::Xml2ObjectWidget(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
	setupUi(this);

	code_hl=new SyntaxHighlighter(code_txt, true);
	code_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
															GlobalAttributes::DIR_SEPARATOR +
															GlobalAttributes::XML_HIGHLIGHT_CONF +
															GlobalAttributes::CONFIGURATION_EXT);
}
