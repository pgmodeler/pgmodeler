# XML definition for procedures
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

<procedure [ name=] "&{name}"

%if {protected} %then [ protected=] "true" %end
%if {sql-disabled} %then [ sql-disabled=] "true" %end
[ security-type=] "{security-type}" > $br

{schema}

%if {owner} %then {owner} %end
%if {comment} %then {comment} %end
%if {appended-sql} %then {appended-sql} %end
%if {prepended-sql} %then {prepended-sql} %end
%if {language} %then {language} %end

%if {transform-types} %then
	$tb [<transform-types names=] "&{transform-types}" /> $br
%end

%if {config-params} %then
	{config-params}
%end

%if {parameters} %then {parameters} %end

%if {library} %then
	$tb [<definition library=] "&{library}"

	%if {symbol} %then
		[ symbol=] "&{symbol}"
	%end

	/> $br
%else
	$tb <definition> $sp <! $ob CDATA $ob {definition} $cb $cb > $sp </definition> $br
%end

</procedure> $br $br
