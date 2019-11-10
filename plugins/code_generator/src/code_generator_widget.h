#ifndef CODE_GENERATOR_WIDGET_H
#define CODE_GENERATOR_WIDGET_H

#include <string>
#include "ui_code_generator_widget.h"
#include "syntaxhighlighter.h"
#include "databasemodel.h"
#include "operationlist.h"
#include "base_code_generator.h"
#include "base_logger.h"

class CodeGeneratorWidget: public QDialog, public Ui::CodeGeneratorWidget, public BaseLogger {
    private:
        Q_OBJECT
        SyntaxHighlighter *code_hl;
        DatabaseModel *model;
        OperationList *op_list;
        BaseCodeGenerator *generator;

    public:
        CodeGeneratorWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
        void setGenerator(BaseCodeGenerator *generator);
        void log(std::string text);

    public slots:
        void show(DatabaseModel *model, OperationList *op_list);

    private slots:
        void doClearCode(void);
        void doGenerateCode(void);
};

#endif
