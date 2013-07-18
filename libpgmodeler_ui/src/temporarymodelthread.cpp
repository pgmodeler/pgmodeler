#include "temporarymodelthread.h"

TemporaryModelThread::TemporaryModelThread(QObject *parent) : QThread(parent)
{
	model_wgt=nullptr;
}

void TemporaryModelThread::setModel(ModelWidget *model)
{
	model_wgt=model;
	this->quit();
}

void TemporaryModelThread::run(void)
{
	if(model_wgt)
	{
		model_wgt->getDatabaseModel()->saveModel(model_wgt->getTempFilename(), SchemaParser::XML_DEFINITION);
		this->quit();
	}
}
