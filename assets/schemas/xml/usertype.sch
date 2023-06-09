# XML definition for user defined types
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
[<usertype name=] "&{name}"

$sp
%if {base} %then configuration="base" %end
%if {composite} %then configuration="composite" %end
%if {enumeration} %then configuration="enumeration" %end
%if {range} %then configuration="range" %end

%if {base} %then
	$br
	[ internal-length=] "{internal-length}"
	[ by-value=] %if {by-value} %then "true" %else "false" %end
	$br

	[ alignment=] "{alignment}"
	[ storage=] "{storage}"
	$br

	%if {element} %then
		[ element=] "&{element}"
	%end

	%if {delimiter} %then
		[ delimiter=] "{delimiter}"
	%end

	$br

	%if {default-value} %then
		[ default-value=] "&{default-value}"
	%end

	%if {category} %then
		[ category=] "{category}"
	%end

	%if {preferred} %then
		[ preferred=] "true"
	%end

	%if {collatable} %then
		[ collatable=] "true"
	%end
%end


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

%if {enumeration} %and {labels} %then
	{labels}
%end

%if {typeattrib} %then {typeattrib} %end


%if {range} %then
	%if {subtype} %then {subtype} %end
	%if {collation} %then {collation} %end
	%if {opclass} %then $tb {opclass} %end
	%if {canonical} %then {canonical} %end
	%if {subtypediff} %then {subtypediff} %end
%end

%if {base} %then
	%if {like-type} %then {like-type} %end
	%if {input} %then {input} %end
	%if {output} %then {output} %end
	%if {receive} %then {receive} %end
	%if {send} %then {send} %end
	%if {tpmodin} %then {tpmodin} %end
	%if {tpmodout} %then {tpmodout} %end
	%if {analyze} %then {analyze} %end
%end

</usertype> $br $br
