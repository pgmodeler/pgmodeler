# SQL definition for procedural languages
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

[CREATE ] 
  %if {trusted} %then 
    %if {handler} %then
      [TRUSTED ]
    %end
  %end
[ LANGUAGE ] {name}

%if {handler} %then
 $br $tb [HANDLER ] {handler}
%end

%if {validator} %then
 $br [VALIDATOR ] {validator}
%end

%if {inline} %then
 $br [INLINE ] {inline}
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
