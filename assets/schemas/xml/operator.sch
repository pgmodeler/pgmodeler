# XML definition for schemas
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
%if {reduced-form} %then $tb %end

[<operator ]

%if {reduced-form} %then
	%if {ref-type} %then ref-type="{ref-type}" %end
	[ signature=] "&{signature}"
%else
	name="&{name}"
%end

%if {reduced-form} %then
	/> $br
%else
	%if {protected} %then
		[ protected=] "true"
	%end

	%if {sql-disabled} %then
		[ sql-disabled=] "true"
	%end

	> $br

	{schema}
	%if {owner} %then {owner} %end
	%if {comment} %then {comment} %end
	%if {appended-sql} %then {appended-sql} %end
	%if {prepended-sql} %then {prepended-sql} %end

	%if {left-type} %then {left-type} %end
	%if {right-type} %then {right-type} %end


	%if {commutator-op} %then {commutator-op} %end
	%if {negator-op} %then {negator-op} %end
	%if {operfunc} %then {operfunc} %end
	%if {join} %then {join} %end
	%if {restriction} %then {restriction} %end

	</operator> $br $br
%end
