# SQL definition for collations
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} != "9.0") %then

 [-- object: ] {name} [ | type: ] {sql-object} [ --] $br
 [-- ] {drop}
 
 # This is a special token that pgModeler recognizes as end of DDL command
 # when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
 %set {ddl-end} $br [-- ddl-end --] $br

 %if {prepended-sql} %then
   {prepended-sql}
   {ddl-end} $br
 %end

 [CREATE COLLATION ] {name}

  %if {collation} %then
    [ FROM ] {collation}
  %else
    [ (]
      %if {locale} %then
		[LOCALE = ] '{locale}'
      %else

        %if {lc-ctype} %then
         [LC_CTYPE = ]'{lc-ctype}'
        %end

        %if {lc-ctype} %and {lc-collate} %then
         [, ]
        %end

        %if {lc-collate} %then
         [LC_COLLATE =] '{lc-collate}'
        %end
        
      %end
    [)]
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
%end
