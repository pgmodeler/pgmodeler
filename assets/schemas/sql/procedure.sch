# SQL definition for procedures
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} >=f "11.0") %then
	[-- object: ] {name} [ | type: ] {sql-object} [ --] $br
	[-- ] {drop}

	# This is a special token that pgModeler recognizes as end of DDL command
	# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
	%set {ddl-end} $br [-- ddl-end --] $br

	%if {prepended-sql} %then
		{prepended-sql}
		{ddl-end} $br
	%end

	[CREATE OR REPLACE PROCEDURE ] {name} $sp ( %if {parameters} %then {parameters} %end ) $br
	$tb [LANGUAGE ] {language} $br

	%if {transform-types} %then
		$tb TRANSFORM {transform-types} $br
	%end

	$tb {security-type} $br

	%if {config-params} %then
		{config-params}
	%end

	$tb [AS ]

	%if {library} %then
		'{library}'

		%if {symbol} %then
			[, ] '{symbol}'
		%end
	%else
		[$$]

		$br {definition} $br
		[$$]
	%end

	;

	{ddl-end}

	%if {owner} %then {owner} %end
	%if {comment} %then {comment} %end

	%if {appended-sql} %then
		{appended-sql}
		{ddl-end}
	%end

	$br
%end
