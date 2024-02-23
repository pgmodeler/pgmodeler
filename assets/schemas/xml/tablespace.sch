# XML definition for tablespaces
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
%if {reduced-form} %then $tb %end
[<tablespace name=] "&{name}"

%if {reduced-form} %then
	/> $br
%else
	[ directory=] "&{directory}"

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
	</tablespace> $br $br
%end
