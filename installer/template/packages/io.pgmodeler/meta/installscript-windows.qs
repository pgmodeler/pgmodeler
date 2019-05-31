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

		mime_update=installdir + "/" + "pgmodeler-cli.exe -mt";    
	    component.addOperation("Execute", "{0,255}", mime_update, "uninstall");
	    component.addOperation("Execute", mime_update, "install", "errormessage=** Could not install file association.");
	    
    } catch (e) {
        print(e);
    }
}

// called after everything is set up, but before any file is written
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
		page.FinishMessageWidget.textEdit.visible=false;
        page.FinishMessageWidget.label.visible=false;
    }
}
