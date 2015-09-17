/**
\ingroup code_generator_plugin
\class CodeGeneratorPlugin
\brief Code generator plugin for pgModeler
*/

#ifndef CODE_GENERATOR_PLUGINH
#define CODE_GENERATOR_PLUGIN_H

#include "pgmodelerplugin.h"

class CodeGeneratorPlugin: public QObject, public PgModelerPlugin {
    private:
        Q_OBJECT

        Q_PLUGIN_METADATA(IID "br.com.pgmodeler.PgModelerPlugin" FILE "code_generator.json")

        //! \brief Declares the interface which is used to implement the plugin
        Q_INTERFACES(PgModelerPlugin)

        void *widget;
        void *generator;

    public:
        CodeGeneratorPlugin(void);
        ~CodeGeneratorPlugin(void);

        QString getPluginTitle(void);
        QString getPluginVersion(void);
        QString getPluginAuthor(void);
        QString getPluginDescription(void);
        QKeySequence getPluginShortcut(void);
        void executePlugin(ModelWidget *);

    public slots:
        void showPluginInfo(void);
};

#endif
