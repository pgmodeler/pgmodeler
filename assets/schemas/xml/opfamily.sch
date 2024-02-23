# XML definition for operator families
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {reduced-form} %then $tb %end

[<opfamily ]

%if {reduced-form} %then
	[signature=] "&{signature}"/> $br
%else
	[name=] "&{name}" [ index-type=] "{index-type}"

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
	</opfamily> $br $br
%end
