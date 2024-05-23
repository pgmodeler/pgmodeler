# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

\n [<a name="] {schema}.{name} ["></a>] \n
\#\#\#\s _{schema}_.**{name}** \s `{type}` \n

{comment}

%if {columns} %then
	[| Name | Data type ] \n
	[| --- | --- |] \n
%else
	[ | No columns | ] \n
	[ | --- |] \n
%end 

%if {objects} %then
	{objects}
%end

%if {references} %then
	\n\#\#\#\#\s References \n

	\n [| Tables |] \n
	[| --- |] \n
	[| ] {references} [ |] \n
%end

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
