function Component()
{
    // Defining default target dirs (used in setInstallDir function)
	if (systemInfo.productType === "windows") {
		installer.setValue("defaultTarget", "C:\\Program Files\\pgModeler");
	}
	else {
		installer.setValue("defaultTarget", "/opt/pgModeler");
	}
	
	// Connecting a slot that handles installationFinished signal
	installer.installationFinished.connect(this, finishInstall);
    
    // Adding custom widgets to the TargetDirectory and InstallationFinished page
    installer.addWizardPageItem(component, "BeginMessageWidget", QInstaller.Introduction);
	installer.addWizardPageItem(component, "ExtraOptionsWidget", QInstaller.TargetDirectory);
    installer.addWizardPageItem(component, "FinishMessageWidget", QInstaller.InstallationFinished);

	var pageTarget = gui.pageById(QInstaller.TargetDirectory);
	
    // Connecting several slots to the clicked signals of the custom radio/checkbox in ExtraOptionsWidget (inserted into TargetDirectory page)
	pageTarget.ExtraOptionsWidget.current_user_rb.clicked.connect(this, setInstallDir);
	pageTarget.ExtraOptionsWidget.all_users_rb.clicked.connect(this, setInstallDir);	
	pageTarget.ExtraOptionsWidget.current_user_rb.clicked.connect(this, setExtraOptions);
	pageTarget.ExtraOptionsWidget.all_users_rb.clicked.connect(this, setExtraOptions);	
	pageTarget.ExtraOptionsWidget.associate_dbm_chk.clicked.connect(this, setExtraOptions);
	pageTarget.ExtraOptionsWidget.create_start_menu_chk.clicked.connect(this, setExtraOptions);	
	
    // If we are running outside Windows we hide the start menu creation option
	if (systemInfo.productType !== "windows") {
		pageTarget.ExtraOptionsWidget.create_start_menu_chk.visible=false;
		pageTarget.ExtraOptionsWidget.create_start_menu_chk.checked=false;
	}
	

	// Hiding the component selection page since we just have one default component to install
	installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    
    // Initialize extra options
	setExtraOptions();
}

Component.prototype.isDefault = function()
{
    return true;
}

Component.prototype.createOperations = function()
{
    try {
        // Call the base create operations function
        component.createOperations();
	
        var installdir = installer.value("TargetDir");
        var mime_update = installdir + "/" + "pgmodeler-cli";

        if(systemInfo.productType === "osx") {
            return;
        }
        else if (systemInfo.productType === "windows") 
		{
            mime_update += ".exe";  
            
            if(installer.value("create_start_menu")  === "true")
            {   
                var startmenu_path = "";

                // Configure the path to the start menu for all users                
                if(installer.value("all_users") === "true") {                    
                    var startmenu = installer.value("StartMenuDir").slice(installer.value("StartMenuDir").lastIndexOf("\\"), 
                                                                          installer.value("StartMenuDir").length);
                    startmenu_path = installer.value("AllUsersStartMenuProgramsPath") + startmenu;
                }
                // Using the default variable StartMenuDir when the installation is only for the current user
                else {
                    startmenu_path = installer.value("StartMenuDir");
                }
                
                // Create the start menu as well the shortcut
                component.addElevatedOperation("Mkdir", startmenu_path);
                component.addElevatedOperation("CreateShortcut", "@TargetDir@/pgmodeler.exe", 
                                                startmenu_path + "/pgModeler.lnk",		
                                                "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/pgmodeler.exe",
                                                "iconId=0", "description=PostgreSQL Database Modeler");
            }
		}

		// If the file assocation action is set
		if(installer.value("update_mime") === "true")
		{		
            var ignored_errors = "{-1,0,127,255}";

            if(installer.value("all_users")  === "true")
			{					
	  			var param1 = "";
				var param2 = "";

                // In linux we need to pass extra parameters to ensure that the cli will be executed correctly in all_users mode
				if(systemInfo.productType !== "windows") {
					param1 = "-platform";
					param2 = "offscreen";
				}
				
				// Execute the mime update command with privilege elevation
				var inst_params = ["-mt", "install", "-sw", param1, param2, "UNDOEXECUTE", mime_update, "-mt", "uninstall", "-sw", param1, param2];
				component.addElevatedOperation("Execute", ignored_errors, mime_update, inst_params);
			}
			else 
			{
                // Execute the mime update command without privilege elevation
				component.addOperation("Execute", ignored_errors, mime_update, "-mt", "install", "UNDOEXECUTE", mime_update, "-mt", "uninstall");	
			}				
		}
		
    } catch (e) {
        print(e);
    }
}

setInstallDir = function()
{
	var page = gui.pageById(QInstaller.TargetDirectory);

	if(page.ExtraOptionsWidget.all_users_rb.checked) {
		page.TargetDirectoryLineEdit.text =  installer.value("defaultTarget");
	}
	else {
		var install_dir = QDesktopServices.storageLocation(QDesktopServices.HomeLocation) + "/pgModeler";
		
        // Converting the dir separators on Windows
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

    /* On Windows if the user uncheck the start menu creation option we hide the
     * page where it defines the name of the start menu folder */
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
