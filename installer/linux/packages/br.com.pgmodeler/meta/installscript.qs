function Component()
{
    // constructor
    installer.installationFinished.connect(this, finishInstall);
    installer.addWizardPageItem( component, "FinishMessageWidget", QInstaller.InstallationFinished );
}

Component.prototype.isDefault = function()
{
    // select the component by default
    return true;
}

Component.prototype.createOperations = function()
{
    try {
        // call the base create operations function
        component.createOperations();

        var installdir=installer.value("TargetDir");

            executable=installdir + "/" + "start-pgmodeler.sh";
            component.addOperation("Execute", "chmod", "+x", executable, "errormessage=** Could not set executable flag for file " + executable);

    } catch (e) {
        print(e);
    }
}

// called after everything is set up, but before any fie is written
Component.prototype.beginInstallation = function()
{
    // call default implementation which is necessary for most hooks
    // in beginInstallation case it makes nothing
    component.beginInstallation();
}

finishInstall = function()
{
    //Getting the "Finished" page in order to detect if the "Run program" check box is marked
    var page = gui.pageWidgetByObjectName( "FinishedPage" );

    if(installer.status == QInstaller.Success)
    {
        var page = gui.pageWidgetByObjectName( "FinishedPage" );
        var info_txt=page.FinishMessageWidget.textEdit.html.replace("{installdir}",installer.value("TargetDir"))
        page.FinishMessageWidget.textEdit.html=info_txt
    }
}
