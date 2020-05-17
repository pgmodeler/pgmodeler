# Catalog queries for conversions
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [ SELECT cn.oid, cn.conname AS name, 
    ns.nspname AS parent,
    'schema' AS parent_type    
    FROM pg_conversion AS cn 
    LEFT JOIN pg_namespace AS ns ON cn.connamespace = ns.oid ]

  %if {schema} %then
    [ WHERE ns.nspname = ] '{schema}'
  %end

  %if {last-sys-oid} %then
    %if {schema} %then
     [ AND ]
    %else
     [ WHERE ]
    %end
     [ cn.oid ] {oid-filter-op} $sp {last-sys-oid}
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
  
    ( {name-filter} )
  %end  

%else
    %if {attribs} %then
     [SELECT cn.oid, cn.conname AS name, cn.connamespace AS schema, cn.conowner AS owner,
	   pg_encoding_to_char(conforencoding) AS src_encoding,
	   pg_encoding_to_char(contoencoding) AS dst_encoding,
	   cn.conproc::oid AS function, cn.condefault AS default_bool, ]

     ({comment}) [ AS comment ]

     [ FROM pg_conversion AS cn ]

      %if {schema} %then
	[ LEFT JOIN pg_namespace AS ns ON cn.connamespace = ns.oid ]
      %end

      %if {filter-oids} %or {schema} %then
	[ WHERE ]
	  %if {filter-oids} %then
	   [ cn.oid IN (] {filter-oids} )

	    %if {schema} %then
	      [ AND ]
	    %end
	  %end

	  %if {schema} %then
	   [ ns.nspname = ] '{schema}'
	  %end
      %end

      %if {last-sys-oid} %then
	%if  {filter-oids} %or {schema} %then
	 [ AND ]
	%else
	 [ WHERE ]
	%end
	 [ cn.oid ] {oid-filter-op} $sp {last-sys-oid}
      %end

      %if {not-ext-object} %then
	%if {filter-oids} %or {last-sys-oid} %or {schema} %then
	  [ AND ]
	%else
	  [ WHERE ]
	%end
	( {not-ext-object} )
      %end

    %end
%end
