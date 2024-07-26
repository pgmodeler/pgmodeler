# SQL command to drop an object
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"

%if %not {domain} %and {constraint} %or {column} %then
	%if %not {decl-in-table} %then
		[ALTER TABLE ] {table} [ DROP ] {sql-object} [ IF EXISTS ]
	%end
%else
	[DROP ] {sql-object} [ IF EXISTS ]
%end

%if {column} %or {constraint} %and %not {decl-in-table} %or {extension} %or {rule} %or {trigger} %or {policy} %then
	{name}
%else
	{signature}
%end

%if {trigger} %or {rule} %or {policy} %then
	[ ON ] {table}
%end

%if {cascade} %and %not {database} %and %not {tablespace} %and %not {role} %and %not {usermapping} %then
	[ CASCADE]
%end

;

{ddl-end}
