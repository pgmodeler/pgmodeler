# SQL definition for sequence's attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

{alter-cmds}

%if {has-changes} %then
 [ALTER ] {sql-object} $sp {signature} $br

 %if {increment} %then
  $tb [INCREMENT BY ] {increment} $br
 %end

 %if {min-value} %then
  $tb [MINVALUE ] {min-value} $br
 %end

 %if {max-value} %then
  $tb [MAXVALUE ] {max-value} $br
 %end

 %if {start} %then
  $tb [START WITH ] {start} $br
 %end

 %if {cache} %then
  $tb [CACHE ] {cache} $br
 %end

 %if {cycle} %then
   $tb 
   %if ({cycle}=="unset") %then NO %end
   CYCLE $br
 %end

 %if {owner-col} %then 
   $tb [OWNED BY ]
  
    %if ({owner-col}=="unset") %then
      NONE
    %else
      {owner-col}
    %end
 %end
 ; $br

 %if {owner-col} %and ({owner-col}!="unset") %then
  $br
  [ALTER TABLE ] {table} [ ALTER COLUMN ] {column} $br
  [      SET DEFAULT nextval('] {name} ['::regclass);] $br
 %end
 
 [-- ddl-end --] $br
%end
