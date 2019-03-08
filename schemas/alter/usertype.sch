# SQL definition for user defined types' attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} != "9.0") %and {has-changes} %then 
  %set {alter-type} [ALTER ] {sql-object} $sp {name}
  %set {ddl-end} ; $br [-- ddl-end --] $br

  %if {value} %then
    {alter-type} 
    [ ADD VALUE ] '{value}'
    
    %if {before} %then 
      [ BEFORE ]
    %else
      [ AFTER ]
    %end
    
    '{existing-value}'
    
    {ddl-end}
  %end

  %if {attribute} %then
    {alter-type}

    %if {drop} %then
      [ DROP ATTRIBUTE ] {attribute} 
    %else
       %if {change} %then
	[ ALTER ATTRIBUTE ] {attribute} [ TYPE ] {type}
       %else
	[ ADD ATTRIBUTE ] {attribute} $sp {type}
      
	%if {collation} %then
	  [ COLLATE ] {collation}
	%end
       %end
    %end

    [ CASCADE]

    {ddl-end}
  %end
%end
