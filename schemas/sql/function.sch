# SQL definition for functions
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] {name} [ | type: ] {sql-object} [ --] $br
[-- ] {drop}

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
%set {ddl-end} $br [-- ddl-end --] $br

%if {prepended-sql} %then
  {prepended-sql}
  {ddl-end} $br
%end

[CREATE FUNCTION ] {name} $sp ( %if {parameters} %then {parameters} %end ) $br
$tb [RETURNS ]

%if {return-table} %then
 [TABLE ] ( {return-table} )
%else
 %if {returns-setof} %then [SETOF ] %end
 {return-type}
%end
$br


$tb [LANGUAGE ] {language} $br

%if {window-func} %then
 $tb WINDOW $br
%end

$tb {function-type} $sp %if {leakproof} %then LEAKPROOF %end $br

$tb {behavior-type} $br
$tb {security-type} $br
$tb [COST ] {execution-cost} $br

%if {returns-setof} %then
 $tb [ROWS ] {row-amount} $br
%end

$tb [AS ]

%if {library} %then
 '{library}'

 %if {symbol} %then
   [, ] '{symbol}'
 %end

%else
[$$]
$br {definition} $br 
[$$]
%end

; 

{ddl-end}

%if {owner} %then {owner} %end
%if {comment} %then {comment} %end

%if {appended-sql} %then
 {appended-sql}
 {ddl-end}
%end

$br
