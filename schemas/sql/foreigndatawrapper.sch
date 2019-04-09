# SQL definition for foreign data wrappers
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[-- object: ] {name} [ | type: ] {sql-object} [ --] $br

[-- ] {drop} 

%if {prepended-sql} %then
    {prepended-sql}
    $br [-- ddl-end --] $br $br
%end

[CREATE ] {sql-object} $sp {name} 

%if ({pgsql-ver} >=f "9.1") %then
    $br $tb

    %if %not {handler} %then
        [NO HANDLER]
    %else
        [HANDLER ] {handler} 
    %end
%end

$br $tb

%if %not {validator} %then
    [NO VALIDATOR]
%else
    [VALIDATOR ] {validator}
%end

%if {options} %then
    $br $tb [OPTIONS (] {options} )
%end

;

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
$br [-- ddl-end --] $br

%if {comment} %then {comment} %end
%if {owner} %then {owner} %end

%if {appended-sql} %then
    {appended-sql}
    $br [-- ddl-end --] $br
%end

$br
