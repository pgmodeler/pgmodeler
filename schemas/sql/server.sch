# SQL definition for schemas
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] {name} [ | type: ] {sql-object} [ --] $br

[-- ] {drop}

%if {prepended-sql} %then
    {prepended-sql}
    $br [-- ddl-end --] $br $br
%end

[CREATE SERVER ] {name} 
$br $tb 

%if {type} %then
    [ TYPE ] ${type} 
%end

%if {version} %then
    [ VERSION ] ${version} 
%end

$br $tb [FOREIGN DATA WRAPPER ] {fdw}

[-- ddl-end --] $br

%if {owner} %then {owner} %end
%if {comment} %then {comment} %end

%if {appended-sql} %then
    {appended-sql}
    $br [-- ddl-end --] $br
%end

$br
