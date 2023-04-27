# XML definition for event triggers
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
[<eventtrigger name=] "&{name}"

%if {event} %then
	[ event=] "{event}"
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
%if {function} %then {function} %end
%if {filter} %then {filter} %end
%if {appended-sql} %then {appended-sql} %end
%if {prepended-sql} %then {prepended-sql} %end

</eventtrigger> $br $br

