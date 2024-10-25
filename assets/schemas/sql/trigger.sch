# SQL definition for triggers
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE ] 

%if %not {constraint} %then 
	[OR REPLACE ]
%else
	[CONSTRAINT ] 
%end

[TRIGGER ] {name} $br
$tb {firing-type} $sp {events} $br

$tb [ON ] {table} $br

%if {constraint} %then
	%if {ref-table} %then
		$tb [FROM ] {ref-table} $br
	%end

	%if {deferrable} %then
		$tb [DEFERRABLE ] {defer-type} $br
	%else
		$tb [NOT DEFERRABLE ] $br
	%end
%end

%if {old-table-name} %or {new-table-name} %then
	$tb REFERENCING
	%if {old-table-name} %then [ OLD TABLE AS ] {old-table-name} %end
	%if {new-table-name} %then [ NEW TABLE AS ] {new-table-name} %end
	$br
%end

$tb [FOR EACH ] %if {per-line} %then ROW %else STATEMENT %end $br

%if {condition} %then $tb WHEN $sp ({condition}) $br %end

$tb [EXECUTE PROCEDURE ] {trigger-func}(
%if {arguments} %then {arguments} %end );

@include "footer"
