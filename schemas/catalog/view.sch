# Catalog queries for views
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [SELECT vw.oid, vw.relname AS name FROM pg_class AS vw ]

  %if {schema} %then
    [ LEFT JOIN pg_namespace AS ns ON ns.oid=vw.relnamespace ]
  %end
      
  %if ({pgsql-ver} <= "9.2") %then
    [ WHERE vw.relkind='v']
  %else
    [ WHERE vw.relkind IN ('v','m') ]
  %end
  
  %if {schema} %then
    [ AND ns.nspname= ] '{schema}'
  %end

  %if {last-sys-oid} %then
    [ AND vw.oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if {not-ext-object} %then
    [ AND ] ( {not-ext-object} )
  %end

%else
    %if {attribs} %then
      [SELECT vw.oid, vw.relname AS name, vw.relnamespace AS  schema, vw.relowner AS owner, vw.reltablespace AS tablespace,
	      vw.relacl AS permission, pg_get_viewdef(vw.oid) AS definition, ]  
	      
      [ CASE
	  WHEN vw.relkind='m' THEN TRUE
	  ELSE FALSE
        END AS materialized_bool, ]   
        

      ({comment}) [ AS comment ]

      [ FROM pg_class AS vw
	LEFT JOIN pg_namespace AS ns ON ns.oid = vw.relnamespace ]
	
      %if ({pgsql-ver} <= "9.2") %then
       [ WHERE vw.relkind='v']
      %else
       [ WHERE vw.relkind IN ('v','m') ]
      %end

       %if {last-sys-oid} %then
	[ AND vw.oid ] {oid-filter-op} $sp {last-sys-oid}
       %end

       %if {not-ext-object} %then
	 [ AND ] (  {not-ext-object} )
       %end

	%if {filter-oids} %or {schema} %then
	[ AND ]
	  %if {filter-oids} %then
	   [ vw.oid IN (] {filter-oids} )

	    %if {schema} %then
	      [ AND ]
	    %end
	  %end

	  %if {schema} %then
	   [ ns.nspname= ] '{schema}'
	  %end
       %end
    %end
%end
