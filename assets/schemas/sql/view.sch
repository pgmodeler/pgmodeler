# SQL definition for views
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

[-- object: ] {name} [ | type: ] {sql-object} [ --] $br
[-- ] {drop}

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
%set {ddl-end} $br [-- ddl-end --] $br

%if {prepended-sql} %then
	{prepended-sql}
	{ddl-end} $br
%end



%if {materialized} %then
[CREATE ]
%else
[CREATE OR REPLACE ]
%end

%if {recursive} %then
	[RECURSIVE ]
%else
	%if {materialized} %then
		[MATERIALIZED ]
	%end
%end

VIEW $sp {name}

%if {columns} %then
	[ (] {columns} [)] 
%end

%if {materialized} %and {tablespace} %then
	$br TABLESPACE $sp {tablespace}
%end

%if {options} %then
	$br [WITH (] {options} [)] 
%end

$br [AS ] $br {definition}

%if {materialized} %and {with-no-data} %then
	$br [WITH NO DATA]
%end

%if %not {materialized} %and {check-option} %then
	$br [WITH ] {check-option} [ OPTION] 
%end

[;] {ddl-end}

%if {comment} %then {comment} %end
%if {owner} %then {owner} %end

%if {appended-sql} %then
	{appended-sql}
	{ddl-end}
%end

$br
