# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

[| ]  {name} [ | ] {type}

%if ({parent} != "view") %then
	%if ({parent} == "table") %then
		[ | ] {pk-constr}
		[ | ] {fk-constr}
		[ | ] {uq-constr}
	%end

	[ | ] {not-null}
	[ | ] {default-value}
	[ | ] {comment}
%end

[ |] \n
