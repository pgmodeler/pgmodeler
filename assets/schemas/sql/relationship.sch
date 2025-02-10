# SQL definition for relationships
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {rel1n} %then
	{constraints}
%end

%if {relnn} %then
	{table}
	{constraints}
%end

%if {rel1n} %or {relnn} %and {index} %then
	{index}
%end

