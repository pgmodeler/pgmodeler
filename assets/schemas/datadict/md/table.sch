# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

\n [<a name="] {schema}.{name} ["></a>] \n
\#\#\#\s _{schema}_.**{name}** \s `{type}` \n

%if {comment} %then
	\n {comment} \n\n
%end

[| Name | Data type ] 

%if {columns} %then

	%if ({type-class} == "table") %then
		[ | PK | FK | UQ ] 
	%end

	[ | Not null | Default value | Description |] \n
	[| --- | --- | :---: | :---: | :---: | :---: | --- | --- |] \n
%else
	[ | No columns | ] \n
	[| --- | --- | --- |] \n
%end 

%if {objects} %then
	{objects}
%end

%if {inherit} %or {partitioned-table} %or {partition-tables} %then
	\n\#\#\#\#\s Inhiritances [ & ] Partitions \n

	\n [| | Tables |] \n
	[| --- | --- |] \n
	
	%if {inherit} %then
		[| Inherits: | ] {inherit} [ |] \n
	%end

	%if {partitioned-table} %then
		[| Partition of: | ] {partitioned-table} [ |] \n
	%end

	%if {partition-tables} %then
		[| Partition(s): | ] {partition-tables} [ |] \n
	%end
%end

@include "navigation"
