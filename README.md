![pgmodeler_mainwindow](https://user-images.githubusercontent.com/2205476/213446508-9bd549b3-ee7f-476d-9249-f537c31fce04.png)


[![Linux build](https://github.com/pgmodeler/pgmodeler/workflows/Linux%20build/badge.svg)](https://github.com/pgmodeler/pgmodeler/actions?query=workflow%3A%22Linux+build%22)
[![Windows build](https://github.com/pgmodeler/pgmodeler/workflows/Windows%20build/badge.svg)](https://github.com/pgmodeler/pgmodeler/actions?query=workflow%3A%22Windows+build%22)
[![macOs build](https://github.com/pgmodeler/pgmodeler/workflows/macOs%20build/badge.svg)](https://github.com/pgmodeler/pgmodeler/actions?query=workflow%3A%22macOs+build%22)

:rocket: What's pgModeler?
------------

An **open-source, multiplatform database modeler for PostgreSQL**. This project aims to be a reference database design tool when it comes to FOSS in the PostgreSQL ecosystem.
Its feature-rich interface allows quick data modeling and fast code deployment on a server. It also supports reverse engineering by creating a visual representation of existing databases. Besides, pgModeler can also generate SQL scripts to sync a model and a database through the process called diff.
This tool is not about modeling only, it also counts with a minimalist but functional database server administration module that allows the execution of any sort of SQL commands, and provides database browsing and data handling in a simple and intuitive UI.

For more details about additional features, screenshots, and other useful information, please, visit the [project's official website](https://pgmodeler.io). For any feedback about the software visit the [Issues](https://github.com/pgmodeler/pgmodeler/issues) page. Additionally, follow pgModeler on [X](https://x.com/pgmodeler), [Bluesky](https://bsky.app/profile/pgmodeler.io) or [Mastodon](https://mastodon.social/@pgmodeler) and be up-to-date with new features, fixes, and releases.

:hammer_and_wrench: Building & Installing
----------------------

Being a multiplatform software, the building process differs slightly on each supported OS. This way, the installation procedures are fully described in the [Installation](https://www.pgmodeler.io/support/installation) section on the project's website.

You may want to check the [official plugin repository](https://github.com/pgmodeler/plugins) as well for information regarding extending pgModeler's features.

:spiral_notepad: Licensing
---------

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See [LICENSE](https://github.com/pgmodeler/pgmodeler/blob/main/LICENSE) for details.

:heart: pgModeler needs your support!
-------------------

* :moneybag: **Financial support:** a lot of knowledge and effort is being put into this project during the last **17 years** so that a reliable and constantly improved product can be delivered to the community. pgModeler is an independent project and has no sponsorship, living exclusively from [donations](https://pgmodeler.io/#donationForm) and [pre-compiled packages purchases](https://www.pgmodeler.io/purchase). If you like pgModeler and think it deserves a financial contribution, go ahead and help it!

* :man_technologist: **Developers:** pgModeler has reached a state where its solo developer can't handle all the modifications and new feature requests by himself anymore in a reasonable time. So if you know how to code in C++ using the Qt framework, and want to contribute to the development, let me know! I'll be grateful for any help with the project maintenance!

* :speaking_head: **Other contributors:** developer or not you can always help this project by spreading the word about it. Share this repository in your social networks, workplaces, family & friends. The more the people know about pgModeler the bigger will be the support for the project, thus creating a virtuous cycle.

:bookmark_tabs: Changelog
----------

The detailed changelog can be seen on [CHANGELOG.md](https://github.com/pgmodeler/pgmodeler/blob/main/CHANGELOG.md) file.

:card_file_box: Older Releases & Code
-------------------

Older releases of pgModeler can be found on [Sourceforge.net](http://sourceforge.net/projects/pgmodeler).

:bomb: Known Issues
-----------

* Due to the usage of Qt's raster engine to draw objects, the process of handling objects on the canvas tends to get slower as lots of objects are created causing constant CPU usage. Heavy performance degradation can be noticed when dealing with models with ~500+ tables and/or relationships. There're plans to improve the speed of drawing operations, for large models, in future releases. Changing the relationship connection mode and render smoothness options may help in the performance when handling big database models.
 
* The diff process still presents false-positive results due to its limitations. Sometimes, there is the need to run the process twice and/or tweaking the options to get the full changes.

* pgModeler does not fully support the [quoted identifier notation](http://www.postgresql.org/docs/current/static/sql-syntax-lexical.html#SQL-SYNTAX-IDENTIFIERS). The non-printable characters, some special UTF8 characters, some special characters, and upper case letters in the objects' names will automatically trigger the usage of quoted identifier notation. The following characters are considered special in pgModeler's implementation: '_-.@ $:()/<>+*\=~!#%^&|?{}[]`;

* pgModeler can't be compiled with Microsoft Visual Studio due to the use of some GCC and clang extensions.

* Compiling the source using '-Wconversion' (disabled by Qt in its defaults) generates a lot of warnings. These warnings are 99% harmless and there are no plans to fix them in a near future.

* pgModeler can sometimes crash during the export, import, validation or diff processes due to some threaded operations that, depending on the size and the arrangement of the model, cause race conditions between threads. 

* Due to the particularities of executing bundles on macOS, the file association does not work correctly on this system. So it's not possible to open a model file by clicking it on Finder.
