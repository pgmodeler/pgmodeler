#include "temporarymodelthread.h"

TemporaryModelThread::TemporaryModelThread(QObject *parent) : QThread(parent)
{
	model_wgt=nullptr;
	enabled=true;
}

void TemporaryModelThread::setModel(ModelWidget *model)
{
	model_wgt=model;

	if(enabled)
		this->quit();
}

bool TemporaryModelThread::isEnabled()
{
	return(enabled);
}

void TemporaryModelThread::run(void)
{
	if(model_wgt && enabled)
	{
		model_wgt->getDatabaseModel()->saveModel(model_wgt->getTempFilename(), SchemaParser::XML_DEFINITION);
		this->quit();
	}
}

void TemporaryModelThread::setEnabled(bool value)
{
	enabled=value;
}
