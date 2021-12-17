# XML definition for SQL history configurations file
# CAUTION: Do not modify this file unless you know what you are doing.
[<?xml version="1.0" encoding="UTF-8" ?>] $br
[<!--] $br
[ CAUTION: Do not modify this file directly on it's code unless you know what you are doing.] $br
[ Unexpected results may occur if the code is changed deliberately.] $br
[-->] $br

<sql-history> $br

%if {commands} %then
	{commands}
%end

</sql-history> $br
