# SQL definition for user mapping
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] {name} [ | type: ] {sql-object} [ --] $br

[-- ] {drop}

%if {prepended-sql} %then
    {prepended-sql}
    $br [-- ddl-end --] $br $br
%end

[CREATE USER MAPPING FOR ]

%if {owner} %then {owner} %else PUBLIC %end $br

[SERVER ] {server} 

%if {options} %then
 $br [OPTIONS (] {options} ) 
%end

;

$br [-- ddl-end --] $br

%if {comment} %then {comment} %end

%if {appended-sql} %then
    {appended-sql}
    $br [-- ddl-end --] $br
%end

$br
