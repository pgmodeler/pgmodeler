# SQL definition for functions/procedures configuration parameters modification
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if ({value} == "unset") %then
	$br $tb [RESET ] {name}
%else
	$br $tb [SET ] {name} [ = ] {value}
%end

