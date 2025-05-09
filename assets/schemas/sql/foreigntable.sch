# SQL definition for foreign tables
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE FOREIGN TABLE ] {name}

%if {partitioned-table} %then
	$br [PARTITION OF ] {partitioned-table} $sp
%end

%if %not {partitioned-table} %then

	[ (] $br

	%if %not {gen-alter-cmds} %then
		%if {columns} %then
			{columns}
		%end

		%if {inh-columns} %then
			{inh-columns}
		%end

		%if {constraints} %then
			{constraints}
		%end
	%end

	%if %not {constraints} %then $br %end
 )

%else
	%if %not {gen-alter-cmds} %and {partitioned-table} %and {constraints} %then
		[ (] $br {constraints} $br [)]
	%end
%end

%if {partitioned-table} %then
	$br [FOR VALUES ] {partition-bound-expr}
%end

%if {ancestor-table} %then $br [ INHERITS(] {ancestor-table} [)] %end
$br [SERVER ] {server}
%if {options} %then $br [OPTIONS (] {options} [)] %end

;

{ddl-end}

%if {gen-alter-cmds} %then
	%if {columns} %then $br {columns} %end
	%if {constraints} %then $br {constraints} %end
%end

%if {comment} %then {comment} %end
%if {cols-comment} %then {cols-comment} %end
%if {owner} %then {owner} %end

%if {appended-sql} %then
	{appended-sql}
	{ddl-end}
%end

%if {initial-data} %then
	$br {initial-data} $br
%end

$br
