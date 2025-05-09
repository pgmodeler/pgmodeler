# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

{ddl-end}

%if {owner} %and 
	({sql-object} != "TABLESPACE") %and 
	({sql-object} != "DATABASE")  %and 
	({sql-object} != "USER MAPPING") %then 
	{owner} 
%end

%if {comment} %then {comment} %end

%if {appended-sql} %then
	{appended-sql}
	{ddl-end}
%end

$br