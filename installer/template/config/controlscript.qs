function Controller() {
}

Controller.prototype.IntroductionPageCallback = function()
{
    var widget = gui.currentPageWidget(); 
    var msg_label = widget.findChild("MessageLabel");

    widget.title = "pgModeler - PostgreSQL Database Modeler";
    
    // When running as an uninstaller hide the package management and update radio buttons
    if (installer.isUninstaller()) {
        widget.title = "Uninstall pgModeler";
        msg_label.text = "Welcome to <strong>pgModeler</strong> uninstall wizard.";
        
        if (widget != null) {
            widget.findChild("PackageManagerRadioButton").visible = false;
            widget.findChild("UpdaterRadioButton").visible = false;
            widget.findChild("UninstallerRadioButton").visible = true;
        }
    }
    else {
        msg_label.text = "Welcome to <strong>pgModeler</strong> Setup Wizard. <br/><br/><br/><br/>" + 
        "<strong>ATTENTION:</strong> if you have another pgModeler installation (be it a demo or a full version) " + 
        "we recommend that you uninstall it first before proceeding with this setup!";
    }
}


Controller.prototype.TargetDirectoryPageCallback = function()
{
    var widget = gui.currentPageWidget(); 
    var msg_label = widget.findChild("MessageLabel");

    widget.title = "Installation options";
}

Controller.prototype.LicenseAgreementPageCallback = function()
{
    var widget = gui.currentPageWidget(); 
    var msg_label = widget.findChild("MessageLabel");

    widget.title = "License agreement";
}

Controller.prototype.ReadyForInstallationPageCallback = function()
{
    var widget = gui.currentPageWidget(); 
    var msg_label = widget.findChild("MessageLabel");

    widget.title = "Setup is ready!";
}

Controller.prototype.PerformInstallationPageCallback = function()
{
    var widget = gui.currentPageWidget(); 
    var msg_label = widget.findChild("MessageLabel");

    if (installer.isInstaller()) {
        widget.title = "Installing...";
    }
    else {
        widget.title = "Uninstalling...";
    }
}

Controller.prototype.FinishedPageCallback = function()
{
    var widget = gui.currentPageWidget(); 
    var msg_label = widget.findChild("MessageLabel");

    widget.title = "Setup has finished!";
}

