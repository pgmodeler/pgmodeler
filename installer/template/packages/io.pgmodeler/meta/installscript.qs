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

        if(systemInfo.productType === "osx") {
            return;
        }
        else if (systemInfo.productType === "windows") {
		
			component.addOperation("CreateShortcut", "@TargetDir@/pgmodeler.exe", "@StartMenuDir@/pgModeler.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/pgmodeler.exe",
            "iconId=0", "description=PostgreSQL Database Modeler");
			
			//mime_update=installdir + "/" + "pgmodeler-cli.exe";    
			//component.addOperation("Execute", "{-1,0,127,255}", mime_update, "-mt", "uninstall");
			//component.addOperation("Execute", "{-1,0,127,255}", mime_update, "-mt", "install");
		}
		else {			
			start_script=installdir + "/" + "pgmodeler";
			mime_update=installdir + "/" + "pgmodeler-cli";
			component.addOperation("Execute", "{-1,0,127,255}", mime_update, "-mt", "uninstall");
			component.addOperation("Execute", "{-1,0,127,255}", mime_update, "-mt", "install");
		}
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
       /* var page = gui.pageWidgetByObjectName( "FinishedPage" );
		
		if (systemInfo.productType === "windows" || systemInfo.productType === "osx") {
			page.FinishMessageWidget.textEdit.visible=false;
			page.FinishMessageWidget.label.visible=false;
		}
		else {
			var info_txt=page.FinishMessageWidget.textEdit.html.replace("{installdir}",installer.value("TargetDir"));
			page.FinishMessageWidget.textEdit.html=info_txt;
		} */
    }
}
