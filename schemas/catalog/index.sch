# Catalog queries for indexes
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
[SELECT id.indexrelid AS oid, cl.relname AS name, 
        tb.oid::regclass::text AS parent, 'table' AS parent_type
  FROM pg_index AS id
  LEFT JOIN pg_class AS cl ON cl.oid = id.indexrelid 
  LEFT JOIN pg_class AS tb ON id.indrelid = tb.oid ]

 %if {schema} %then
    [ LEFT JOIN pg_namespace AS ns ON ns.oid = tb.relnamespace
      WHERE nspname= ] '{schema}'

   %if {table} %then
     [ AND ((tb.relkind = 'r' OR tb.relkind = 'm') AND tb.relname = ] '{table}' [)]
   %end
 %end

  %if {last-sys-oid} %then
    %if {schema} %then
      [ AND ]
    %else
      [ WHERE ]
    %end
    [ id.indexrelid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if %not {schema} %and %not {last-sys-oid} %then
     [ WHERE ]
   %else
     [ AND ]
   %end

   [ (id.indisprimary IS FALSE ]

   %if ({pgsql-ver} != "9.0") %then
     [ AND id.indisexclusion IS FALSE ]
   %end

   [) AND ((SELECT count(oid) FROM pg_constraint WHERE conindid=id.indexrelid)=0) ]

   %if {not-ext-object} %then
     [ AND ]( {not-ext-object} )
   %end
   
   %if {name-filter} %then
     [ AND ] ( {name-filter} )
   %end
   
%else
    %if {attribs} %then
      [SELECT id.indexrelid AS oid, cl.relname AS name,
	      am.amname AS index_type, id.indrelid AS table,
	      id.indisunique AS unique_bool, ]

      %if ({pgsql-ver} == "9.0") %then
       [ NULL AS collations, ]
      %else
       [ indcollation::oid] $ob $cb [ AS collations, ]
      %end

      [       id.indkey::oid] $ob $cb [ AS columns,
	      id.indclass::oid] $ob $cb [ AS opclasses,
	      pg_get_expr(indexprs, indrelid) AS expressions,
              pg_get_expr(indpred, indrelid, true) predicate, ]
        
        ({comment}) [ AS comment ]
        
      [        
	FROM pg_index AS id
	LEFT JOIN pg_class AS cl ON cl.oid = id.indexrelid
	LEFT JOIN pg_am AS am ON cl.relam  = am.oid
        ]

     %if {schema} %then
	  [ LEFT JOIN pg_class AS tb ON id.indrelid = tb.oid
	    LEFT JOIN pg_namespace AS ns ON ns.oid = tb.relnamespace
	    WHERE ns.nspname= ] '{schema}'

	%if {table} %then
	  [ AND ((tb.relkind = 'r' OR tb.relkind = 'm') AND tb.relname = ] '{table}' [)]
	%end
     %end

     %if {last-sys-oid} %then
	%if {schema} %then
	  [ AND ]
	%else
	  [ WHERE ]
	%end
	[ id.indexrelid ] {oid-filter-op} $sp {last-sys-oid}
     %end

     %if {filter-oids} %then
       %if {schema} %or {last-sys-oid} %then
	 [ AND ]
       %else
	 [ WHERE ]
       %end

       [ id.indexrelid IN (] {filter-oids} )
     %end

     %if %not {schema} %and %not {last-sys-oid} %and %not {filter-oids} %then
       [ WHERE ]
     %else
       [ AND ]
     %end

     [ (id.indisprimary IS FALSE ]

     %if ({pgsql-ver} != "9.0") %then
       [ AND id.indisexclusion IS FALSE ]
     %end

     [) AND ((SELECT count(oid) FROM pg_constraint WHERE conindid=id.indexrelid)=0) ]

      %if {not-ext-object} %then
        [ AND ]( {not-ext-object} )
      %end

    %end
%end
