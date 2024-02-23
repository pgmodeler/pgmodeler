# XML definition for functions
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
%if {reduced-form} %then $tb %end

<function

%if {reduced-form} %then
	%if {ref-type} %then
		[ ref-type=] "{ref-type}"
	%end

	[ signature=] "&{signature}"
%else
	[ name=] "&{name}"
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

	$br
	$tb $tb window-func=%if {window-func} %then "true" %else "false" %end $br

	%if {leakproof} %then $tb $tb leakproof="true" $br %end

	$tb $tb returns-setof=%if {returns-setof} %then "true" %else "false" %end $br
	$tb $tb behavior-type="{behavior-type}" $br
	$tb $tb function-type="{function-type}" $br
	$tb $tb security-type="{security-type}" $br
	$tb $tb parallel-type="{parallel-type}" $br
	$tb $tb execution-cost="{execution-cost}" $br
	$tb $tb row-amount="{row-amount}"
	> $br

	{schema}
	%if {owner} %then {owner} %end
	%if {comment} %then {comment} %end
	%if {appended-sql} %then {appended-sql} %end
	%if {prepended-sql} %then {prepended-sql} %end
	%if {language} %then {language} %end

	$tb <return-type> $br

	%if {return-table} %then
		{return-table}
	%else
		{return-type}
	%end

	$tb </return-type> $br

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

	</function> $br $br
%end
