# Prepended SQL commands attached to objects' defintion.
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {prepended-sql} %then
	{prepended-sql}
	{ddl-end} $br
%end
