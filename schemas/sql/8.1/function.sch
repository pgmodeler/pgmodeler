# SQL definition for functions
# PostgreSQL Version: 8.1
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE OR REPLACE FUNCTION ] $br
$tb @{name} $sp ( %if @{parameters} %then @{parameters} %end ) $br
$tb [RETURNS ]
%if @{returns-setof} %then [SETOF ] %end
@{return-type} $br

$tb [LANGUAGE ] @{language} $br
$tb @{function-type} $br
$tb @{behavior-type} $br
$tb @{security-type} $br

$tb [AS ]

%if @{library} %then
 '@{library}'

 %if @{symbol} %then
   [, ] '@{symbol}'
 %end

%else
[$$]
 @{definition}
[$$]
%end

; $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br