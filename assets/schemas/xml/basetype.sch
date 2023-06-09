# XML definition for native types
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
$tb [<type name=] "&{name}"

%if {length} %then
	[ length=] "{length}"
%end

%if {dimension} %then
	[ dimension=] "{dimension}"
%end

%if {precision} %then
	[ precision=] "{precision}"
%end

%if {ref-type} %then
	[ ref-type=] "{ref-type}"
%end

%if {with-timezone} %then
	[ with-timezone=] "true"
%end

%if {interval-type} %then
	[ interval-type=] "{interval-type}"
%end

%if {spatial-type} %then
	[ spatial-type=] "{spatial-type}"
	[ variation=] "{variation}"
	[ srid=] "{srid}"
%end

/> $br
