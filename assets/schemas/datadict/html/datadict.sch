# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "header"

%if %not {split} %and {datadictidx} %then
	{datadictidx}
%end

{objects}

@include "footer"
