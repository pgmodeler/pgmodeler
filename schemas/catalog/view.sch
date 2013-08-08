# Catalog queries for views
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT vw.oid, vw.relname AS name FROM pg_class AS vw ]

  %if @{schema} %then
    [ LEFT JOIN pg_namespace AS ns ON ns.oid=vw.relnamespace
      WHERE vw.relkind='v' AND ns.nspname= ] '@{schema}'
  %else
    [ WHERE vw.relkind='v']
  %end

   %if @{last-sys-oid} %then
    [ AND vw.oid > ] @{last-sys-oid}
  %end

%else
    %if @{attribs} %then
      [SELECT vw.oid, vw.relname AS name, vw.relnamespace AS  schema, vw.relowner AS owner,
	      vw.relacl AS permissions, _vw1.definition, ]

      (@{comment}) [ AS comment, ]
      (@{from-extension}) [ AS from_extension_bool ]

      [ FROM pg_class AS vw
	LEFT JOIN pg_views AS _vw1 ON _vw1.viewname=vw.relname
	WHERE vw.relkind='v' ]

       %if @{last-sys-oid} %then
	[ AND vw.oid > ] @{last-sys-oid}
       %end

	%if @{filter-oids} %or @{schema} %then
	[ AND ]
	  %if @{filter-oids} %then
	   [ vw.oid IN (] @{filter-oids} )

	    %if @{schema} %then
	      [ AND ]
	    %end
	  %end

	  %if @{schema} %then
	   [ _vw1.schemaname= ] '@{schema}'
	  %end
       %end
    %end
%end
