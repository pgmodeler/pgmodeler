# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {split} %then
	%set {link} {item} [.html]
%else
	%set {link} \# {item}
%end

\n \s <li><a \s href="{link}"> {item} </a></li>
