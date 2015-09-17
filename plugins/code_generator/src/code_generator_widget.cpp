#include "code_generator_widget.h"

#include <string>

CodeGeneratorWidget::CodeGeneratorWidget(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f)
{
    setupUi(this);

    code_hl=new SyntaxHighlighter(code_txt);
    code_hl->loadConfiguration(GlobalAttributes::XML_HIGHLIGHT_CONF_PATH);

    connect(close_btn, SIGNAL(clicked(void)), this, SLOT(close(void)));
    connect(clear_btn, SIGNAL(clicked(void)), this, SLOT(doClearCode(void)));
    connect(generate_btn, SIGNAL(clicked(void)), this, SLOT(doGenerateCode(void)));

    this->generator = 0;
}

void CodeGeneratorWidget::setGenerator(BaseCodeGenerator *generator)
{
    this->generator = generator;
}

void CodeGeneratorWidget::show(DatabaseModel *model, OperationList *op_list)
{
    doClearCode();
    this->setEnabled(model!=nullptr && op_list!=nullptr);
    this->op_list = op_list;
    this->model = model;
    QDialog::show();
}

void CodeGeneratorWidget::doClearCode(void)
{
    code_txt->setPlainText(QString("Ready.."));
}

void CodeGeneratorWidget::doGenerateCode(void)
{
    try
    {
        if(!op_list->isOperationChainStarted())
            op_list->startOperationChain();

        this->generator->generateCode(model, this);

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

void CodeGeneratorWidget::log(std::string text)
{
    code_txt->setPlainText(QString::fromStdString(text));
}
