# SQL definition for indexes
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

[CREATE ] %if {unique} %then [UNIQUE ] %end [INDEX ]

%if {concurrent} %then
	[ CONCURRENTLY ]
%end

{name} [ ON ] {table}

%if {index-type} %then
	$br [USING ] {index-type}
%end

$br ( {elements} $br )

%if {expression} %then
	$br ({expression}) $sp
%end

%if ({pgsql-ver} >=f "11.0") %and {include-cols} %then
	$br [INCLUDE ] ({include-cols})
%end

%if {stg-params} %then
	$br [WITH (]

	%if {factor} %then
		[FILLFACTOR = ] {factor}
	%end

	%if {fast-update} %then
		%if {factor} %then [, ] %end
		[FASTUPDATE = ON]
	%end

	%if {buffering} %and ({pgsql-ver} >=f "9.2") %then
		%if {factor} %then [, ] %end
		[BUFFERING = ON]
	%end

	[)]
%end

%if {tablespace} %then
	$br [TABLESPACE ] {tablespace}
%end

%if {predicate} %then
	$br [WHERE (] {predicate} [)]
%end

;

{ddl-end}

%if {comment} %then {comment} %end

%if {appended-sql} %then
	{appended-sql}
	{ddl-end}
%end

$br
