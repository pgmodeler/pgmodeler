# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%set {spc} \n\#\#\#\s

\n [<a name="index"></a>]
{spc} [Database: ] {name} \n

%if {table} %then
 	{spc} Tables \n 	
 	{table}
%end

%if {foreigntable} %then
	{spc} [Foreign tables] \n 	
 	{foreigntable}
%end

%if {view} %then
	{spc} Views \n
	{view}
%end

%if {split} %then
	@include "footer"
%end
