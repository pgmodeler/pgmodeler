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

$br [-- ddl-end --] $br

%if {owner} %then {owner} %end
%if {comment} %then {comment} %end

%if {appended-sql} %then
    {appended-sql}
    $br [-- ddl-end --] $br
%end

$br
