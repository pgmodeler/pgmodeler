v1.0.0-alpha1
------

<em>Release date: June 21, 2022</em><br/>
<em>Changes since: <strong>v1.0.0-alpha</strong></em><br/>

<strong>Attention:</strong> pgModeler 0.9.x settings are not compatible with pgModeler 1.0.0-alpha1. This way, in the first start of the newer version, some settings will be automatically migrated if the tool detects the configuration files related to 0.9.4 or below in the system! <br/>

<strong>Summary:</strong> since the main focus of this release was to improve even more the UI, we had to work on the refactoring of the codebase to use Qt 6 which has lots of enhancements compared to the previous major version. So, after 2 months of work, pgModeler is now completely ported to Qt 6 and thus not compatible with Qt 5 anymore. <br/>

So, attending to some requests, pgModeler received the ability to change the icon sizes to fit different screens in a better way. Still in the responsiveness subject and using the Qt 6 improved high DPI screens support, pgModeler allows the use of custom UI scale factors which adjust the entire user-interface rendering depending on the screen that the tool is running. <br/>

Another enhancement that is worth mentioning here is that the columns/constraints propagation mechanism was completely rewritten which resulted in a noticeable performance gain and now solves an old problem of columns/constraints created by identifier relationships not being added or even disappearing in certain circumstances. <br/>

Finally, as always, there were a lot of other fixes and changes (56 in total) in almost all parts of the tool which improves stability and reliability. For more details about them, please, read the CHANGELOG.md file. <br/>
