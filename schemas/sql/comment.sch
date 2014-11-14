# SQL definition for comments
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{comment} %then
 [COMMENT ON ] @{sql-object} $sp @{signature} [ IS ] '@{comment}'; $br

 # This is a special token that pgModeler recognizes as end of DDL command
 # when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
 [-- ddl-end --] $br
%end
