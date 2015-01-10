# Catalog queries for collation
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %and ({pgsql-ver} != "9.0") %then
 [SELECT cl.oid, collname AS name FROM pg_collation AS cl ]

  %if {schema} %then
    [ LEFT JOIN pg_namespace AS ns ON cl.collnamespace = ns.oid
       WHERE ns.nspname = ] '{schema}'
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

%else
  %if {attribs} %and ({pgsql-ver} != "9.0")  %then
      [ SELECT cl.oid, cl.collname AS name, cl.collnamespace AS schema, 
               cl.collowner AS owner, pg_encoding_to_char(cl.collencoding) AS encoding,
               cl.collcollate AS lc_collate, cl.collctype AS lc_ctype, ]
               
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
