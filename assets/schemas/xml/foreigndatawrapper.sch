# XML definition for indexes
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {reduced-form} %then
	$tb
%end

[<foreigndatawrapper name=] "&{name}"

%if {reduced-form} %then
	/> $br
%else
	%if {options} %then
		[ options=] "&{options}"
	%end

	%if {protected} %then
		[ protected=] "true"
	%end

	%if {sql-disabled} %then
		[ sql-disabled=] "true"
	%end

	> $br

	%if {owner} %then {owner} %end
	%if {comment} %then {comment} %end
	%if {appended-sql} %then {appended-sql} %end
	%if {prepended-sql} %then {prepended-sql} %end

	%if {handler} %then {handler} %end
	%if {validator} %then {validator} %end

	</foreigndatawrapper> $br $br
%end
