# Catalog queries for collation
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %and ({pgsql-ver} != "9.0") %then
  
  %if {use-signature} %then
    %set {signature} [ ns.nspname || '.' || ]
  %end 

  [SELECT cl.oid, collname AS name, 
   ns.nspname AS parent, 'schema' AS parent_type FROM pg_collation AS cl 
   LEFT JOIN pg_namespace AS ns ON cl.collnamespace = ns.oid]

  %if {schema} %then
    [ WHERE ns.nspname = ] '{schema}'
  %end

  %if {last-sys-oid} %then
    %if {schema} %then
     [ AND ]
   %else
     [ WHERE ]
   %end

   [ cl.oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if {not-ext-object} %then
    %if {last-sys-oid} %or {schema} %then
      [ AND ]
    %else
     [ WHERE ]
    %end
    ( {not-ext-object} )
  %end

  %if {name-filter} %then
  
    %if {last-sys-oid} %or {schema} %or {not-ext-object} %then
      [ AND ]
    %else
      [ WHERE ]
    %end
  
    ( {signature} [ cl.collname ~* ] E'{name-filter}' )
  %end  
  
%else
  %if {attribs} %and ({pgsql-ver} != "9.0")  %then
      [ SELECT cl.oid, cl.collname AS name, cl.collnamespace AS schema, 
               cl.collowner AS owner, pg_encoding_to_char(cl.collencoding) AS encoding,
               cl.collcollate AS lc_collate, cl.collctype AS lc_ctype, 
               split_part(collctype, '@', 2) AS lc_ctype_mod, 
               split_part(collcollate, '@', 2) AS lc_collate_mod, ]
               
      %if ({pgsql-ver} >=f "10.0") %then
        [ CASE  
              WHEN collprovider = 'i' THEN 'icu'
              WHEN collprovider = 'c' THEN 'libc'
              ELSE '' 
          END AS provider, ]
      %else
        [ NULL AS provider, ]
      %end 
      
      %if ({pgsql-ver} >=f "12.0") %then
        [ collisdeterministic AS deterministic_bool, ]
      %else
        [ false AS deterministic_bool, ]
      %end
                
      ({comment}) [ AS comment ]
               
      [ FROM pg_collation AS cl ]

      %if {schema} %then
	[ LEFT JOIN pg_namespace AS ns ON cl.collnamespace = ns.oid ]
      %end

      %if {last-sys-oid} %then
       [ WHERE cl.oid ] {oid-filter-op} $sp {last-sys-oid}
      %end

      %if {not-ext-object} %then
	%if {last-sys-oid} %then
	  [ AND ]
	%else
	  [ WHERE ]
	%end
	( {not-ext-object} )
      %end

      %if {filter-oids} %or {schema} %then
	%if {last-sys-oid} %or {not-ext-object} %then
	 [ AND ]
	%else
	 [ WHERE ]
	%end

	  %if {filter-oids} %then
	   [ cl.oid IN (] {filter-oids} )

	    %if {schema} %then
	      [ AND ]
	    %end
	  %end

	  %if {schema} %then
	   [ ns.nspname = ] '{schema}'
	  %end
      %end
    %end
%end
