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

            start_script=installdir + "/" + "start-pgmodeler.sh";
            mime_update=installdir + "/" + "dbm-mime-type.sh";
            
            component.addOperation("Execute", "chmod", "+x", start_script, "errormessage=** Could not set executable flag for file " + start_script);
            component.addOperation("Execute", "chmod", "+x", mime_update, "errormessage=** Could not set executable flag for file " + mime_update);
            
	    component.addOperation("Execute", "{0,255}", mime_update, "uninstall");
	    component.addOperation("Execute", mime_update, "install", "errormessage=** Could not install file association.");
	    
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
