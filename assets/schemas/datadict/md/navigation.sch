# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {datadictidx} %then
	\n 

 	%if {split} %and {previous} %then
 		\[ \&larr; \s {previous} \] ({previous}.md) \n 
 	%end

	\[ \&uarr; \s Index \] (
		%if {split} %then
			index.md
		%else
			\# index
		%end
	) \n

 	%if {split} %and {next} %then
 		\[ {next} \s \&rarr; \] ({next}.md) \n 
 	%end
%end

\n --- \n