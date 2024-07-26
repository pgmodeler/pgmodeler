# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!

@include "ddlend"

%if {function} %then
	[SET check_function_bodies = false;]
	{ddl-end} $br
%end

%if {search-path} %then
	[SET search_path TO ] {search-path};
	{ddl-end} $br
%end
