# SQL definition for foreign data wrappers
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

[CREATE ] {sql-object} $sp {name} 

%if ({pgsql-ver} >=f "9.1") %then
    $br

    %if %not {handler} %then
        [NO HANDLER]
    %else
        [HANDLER ] {handler} 
    %end
%end

$br

%if %not {validator} %then
    [NO VALIDATOR]
%else
    [VALIDATOR ] {validator}
%end

%if {options} %then
    $br [OPTIONS (] {options} )
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
