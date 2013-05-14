# SQL definition for collations
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{pgsql91} %or @{pgsql92} %then

 [-- object: ] @{name} [ | type: ] @{sql-object} [ --] $br
 [CREATE COLLATION ] @{name}

  %if @{collation} %then
    [ FROM ] @{collation}
  %else
    [ (]
      %if @{locale} %then
		[LOCALE = ] '@{locale}'
      %else

        %if @{lc-ctype} %then
         [LC_CTYPE = ]'@{lc-ctype}'
        %end

        %if @{lc-ctype} %and @{lc-collate} %then
         [, ]
        %end

        %if @{lc-collate} %then
         [LC_COLLATE =] '@{lc-collate}'
        %end
        
      %end
    [)]
  %end

  ; $br

  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end

  # This is a special token that pgModeler recognizes as end of DDL command
  # when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
  [-- ddl-end --] $br $br
%end
