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

	var pageTarget = gui.pageById(QInstaller.TargetDirectory);
	
	pageTarget.ExtraOptionsWidget.current_user_rb.clicked.connect(this, setInstallDir);
	pageTarget.ExtraOptionsWidget.all_users_rb.clicked.connect(this, setInstallDir);	
	pageTarget.ExtraOptionsWidget.current_user_rb.clicked.connect(this, setExtraOptions);
	pageTarget.ExtraOptionsWidget.all_users_rb.clicked.connect(this, setExtraOptions);	
	pageTarget.ExtraOptionsWidget.associate_dbm_chk.clicked.connect(this, setExtraOptions);
	pageTarget.ExtraOptionsWidget.create_start_menu_chk.clicked.connect(this, setExtraOptions);	
	
	if (systemInfo.productType !== "windows") {
		pageTarget.ExtraOptionsWidget.create_start_menu_chk.visible=false;
		pageTarget.ExtraOptionsWidget.create_start_menu_chk.checked=false;
	}
	
	if (installer.isUninstaller()) {
		var pageIntro = gui.pageById(QInstaller.Introduction);		
		
		pageIntro.findChild("PackageManagerRadioButton").visible=false;
		pageIntro.UpdaterRadioButton.visible=false;
	}

	installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
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

            if(installer.value("all_users")  === "true")
			{					
	  			var param1 = "";
				var param2 = "";

				if(systemInfo.productType === "linux") {
					param1 = "-platform";
					param2 = "offscreen";
				}

				component.addElevatedOperation("Execute", ignored_errors, mime_update, "-mt", "uninstall", "-sw", param1, param2);
				component.addElevatedOperation("Execute", ignored_errors, mime_update, "-mt", "install", "-sw", param1, param2);
			}
			else 
			{
				component.addOperation("Execute", ignored_errors, mime_update, "-mt", "uninstall");
				component.addOperation("Execute", ignored_errors, mime_update, "-mt", "install");	
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
		var install_dir = QDesktopServices.storageLocation(QDesktopServices.HomeLocation) + "/pgModeler";
		
		if (systemInfo.productType === "windows") {
			install_dir =  install_dir.replace(/\//g,'\\');
		}
		
		page.TargetDirectoryLineEdit.text = install_dir;
	}
	
}

setExtraOptions = function()
{
	var wgt = gui.pageById(QInstaller.TargetDirectory).ExtraOptionsWidget;
	
	installer.setValue("all_users", wgt.all_users_rb.checked);
	installer.setValue("update_mime", wgt.associate_dbm_chk.checked);
	installer.setValue("create_start_menu", wgt.create_start_menu_chk.checked);	  

	if (systemInfo.productType === "windows") 
	{
		installer.setDefaultPageVisible(QInstaller.StartMenuSelection, wgt.create_start_menu_chk.checked);
	}
}

finishInstall = function()
{
    var page = gui.pageById(QInstaller.InstallationFinished);
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
