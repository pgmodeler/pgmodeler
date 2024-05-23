# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {split} %then
	%set {link} {name} [.md]
%else
	%set {link} \# {name}
%end

\[ {name} \] ({link}) 