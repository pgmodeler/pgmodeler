function Component()
{
    // constructor
	if (systemInfo.productType === "windows") {
		installer.setValue("defaultTarget", "C:\\Program Files\\pgModeler");
	}
	else {
		installer.setValue("defaultTarget", "/opt/pgModeler");
	}
	
	installer.installationFinished.connect(this, finishInstall);
	installer.addWizardPageItem( component, "ExtraOptionsWidget", QInstaller.TargetDirectory);
    installer.addWizardPageItem( component, "FinishMessageWidget", QInstaller.InstallationFinished );

	gui.pageById(QInstaller.TargetDirectory).ExtraOptionsWidget.current_user_rb.clicked.connect(this, setInstallDir);
	gui.pageById(QInstaller.TargetDirectory).ExtraOptionsWidget.all_users_rb.clicked.connect(this, setInstallDir);
	
	gui.pageById(QInstaller.TargetDirectory).ExtraOptionsWidget.current_user_rb.clicked.connect(this, setExtraOptions);
	gui.pageById(QInstaller.TargetDirectory).ExtraOptionsWidget.all_users_rb.clicked.connect(this, setExtraOptions);
	
	gui.pageById(QInstaller.TargetDirectory).ExtraOptionsWidget.associate_dbm_chk.clicked.connect(this, setExtraOptions);
	gui.pageById(QInstaller.TargetDirectory).ExtraOptionsWidget.create_start_menu_chk.clicked.connect(this, setExtraOptions);	
	
	if (systemInfo.productType !== "windows") {
		gui.pageById(QInstaller.TargetDirectory).ExtraOptionsWidget.create_start_menu_chk.visible=false;
		gui.pageById(QInstaller.TargetDirectory).ExtraOptionsWidget.create_start_menu_chk.checked=false;
	}

	setExtraOptions();
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
	
        var installdir = installer.value("TargetDir");
        var mime_update = installdir + "/" + "pgmodeler-cli";

        if(systemInfo.productType === "osx") {
            return;
        }
        else if (systemInfo.productType === "windows") 
		{
            mime_update += ".exe";  
            
			var startmenu = installer.value("StartMenuDir").slice(installer.value("StartMenuDir").lastIndexOf("\\"), installer.value("StartMenuDir").length);
			var startmenu_path = installer.value("AllUsersStartMenuProgramsPath") + startmenu;
			
            if(installer.value("create_start_menu")  === "true")
            {
                
                component.addElevatedOperation("Mkdir", startmenu_path);
                component.addElevatedOperation("CreateShortcut", "@TargetDir@/pgmodeler.exe", 
                                                startmenu_path + "/pgModeler.lnk",		
                                                "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/pgmodeler.exe",
                                                "iconId=0", "description=PostgreSQL Database Modeler");
            }
		}
		else 
		{						
			mime_update=installdir + "/" + "pgmodeler-cli";
		}

		if(installer.value("update_mime") === "true")
		{		
            var ignored_errors = "{-1,0,127,255}";
  			var param1 = "-platform";
			var param2 = "offscreen";
			var param3 = "-mt";

            if(installer.value("all_users")  === "true")
			{					
				component.addElevatedOperation("Execute", ignored_errors, mime_update, param1, param2, param3, "uninstall", "-sw");
				component.addElevatedOperation("Execute", ignored_errors, mime_update, param1, param2, param3, "install", "-sw");
			}
			else 
			{
				component.addOperation("Execute", ignored_errors, mime_update, param1, param2, param3, "uninstall");
				component.addOperation("Execute", ignored_errors, mime_update, param1, param2, param3, "install");	
			}				
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

setInstallDir = function()
{
	var page = gui.pageById(QInstaller.TargetDirectory);
	
	if(page.ExtraOptionsWidget.all_users_rb.checked) {
		page.TargetDirectoryLineEdit.text =  installer.value("defaultTarget");
	}
	else {
		page.TargetDirectoryLineEdit.text = QDesktopServices.storageLocation(QDesktopServices.HomeLocation) + "/pgModeler";
	}
	
}

setExtraOptions = function()
{
	var wgt = gui.pageById(QInstaller.TargetDirectory).ExtraOptionsWidget;
	installer.setValue("all_users", wgt.all_users_rb.checked);
	installer.setValue("update_mime", wgt.associate_dbm_chk.checked);
	installer.setValue("create_start_menu", wgt.create_start_menu_chk.checked);	    
}

finishInstall = function()
{
    var page = gui.pageWidgetByObjectName( "FinishedPage" );
	var label = page.FinishMessageWidget.label;
	
	if(installer.status !== QInstaller.Success) {
		label.styleSheet="color: red";
	}
	
    if(installer.status === QInstaller.Failure)
    {			
		page.FinishMessageWidget.label.text="Failed to install pgModeler!";
    }
    else if(installer.status === QInstaller.Canceled) 
	{
		page.FinishMessageWidget.label.text="Installation canceled by the user!";
	}
}
