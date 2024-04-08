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

[CREATE ]

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

$br

%if {materialized} %and {tablespace} %then
	TABLESPACE $sp {tablespace} $br
%end

[AS ] $br {definition}

%if {materialized} %and {with-no-data} %then
	$br [WITH NO DATA]
%end

[;] {ddl-end}

%if {comment} %then {comment} %end
%if {owner} %then {owner} %end

%if {appended-sql} %then
	{appended-sql}
	{ddl-end}
%end

$br
