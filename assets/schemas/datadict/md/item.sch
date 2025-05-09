# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {split} %then
	%set {link} {item} [.md]
%else
	%set {link} \# {item}
%end

* \s \[ {item} \] ({link}) \n
