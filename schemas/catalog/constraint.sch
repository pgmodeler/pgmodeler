# Catalog queries for constraints
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
[ SELECT cs.oid, cs.conname AS name FROM pg_constraint AS cs ]

 %if @{schema} %then
   [ LEFT JOIN pg_namespace AS ns ON ns.oid = cs.connamespace
     LEFT JOIN pg_class AS tb ON cs.conrelid = tb.oid
     WHERE nspname= ] '@{schema}'

   %if @{table} %then
     [ AND relkind='r' AND relname=] '@{table}'
   %end
 %end

  %if @{last-sys-oid} %then
    %if @{schema} %then
      [ AND ]
    %else
      [ WHERE ]
    %end
    [ cs.oid > ] @{last-sys-oid}
  %end

%else
    %if @{attribs} %then
     [SELECT cs.oid, cs.conname AS name, cs.conrelid AS table, ds.description AS comment,
	     cs.conkey AS src_columns, cs.confkey AS dst_columns, cs.consrc AS expression,
	     cs.condeferrable AS deferrable_bool, cs.confrelid AS ref_table,
	     cl.reltablespace AS tablespace, cs.conexclop AS operators,
	     cl.relam AS index_type, ]

     %if %not @{pgsql92} %then
     [ FALSE AS no_inherit_bool, ]
     %else
     [ cs.connoinherit AS no_inherit_bool, ]
     %end


     [	CASE cs.contype
	  WHEN 'p' THEN 'pk-constr'
	  WHEN 'u' THEN 'uq-constr'
	  WHEN 'c' THEN 'ck-constr'
	  WHEN 'f' THEN 'fk-constr'
	  ELSE 'ex-constr'
	END AS type,

	CASE cs.condeferred
	  WHEN TRUE THEN 'INITIALLY DEFERRED'
	  ELSE 'INITIALLY IMMEDIATE'
	END AS defer_type,

	CASE cs.confupdtype
	  WHEN 'a' THEN 'NO ACTION'
	  WHEN 'r' THEN 'RESTRICT'
	  WHEN 'c' THEN 'CASCADE'
	  WHEN 'n' THEN 'SET NULL'
	  WHEN 'd' THEN 'SET DEFAULT'
	  ELSE NULL
	END AS upd_action,

	CASE cs.confdeltype
	  WHEN 'a' THEN 'NO ACTION'
	  WHEN 'r' THEN 'RESTRICT'
	  WHEN 'c' THEN 'CASCADE'
	  WHEN 'n' THEN 'SET NULL'
	  WHEN 'd' THEN 'SET DEFAULT'
	  ELSE NULL
	END AS del_action,

	CASE cs.confmatchtype
	  WHEN 'f' THEN 'MATCH FULL'
	  WHEN 'p' THEN 'MATCH PARTIAL'
	  WHEN 'u' THEN 'MATCH SIMPLE'
	  ELSE NULL
	END AS comparision_type

     FROM pg_constraint AS cs
     LEFT JOIN pg_description AS ds ON ds.objoid=cs.oid
     LEFT JOIN pg_class AS cl ON cl.oid = cs.conindid ]

     %if @{schema} %then
	[ LEFT JOIN pg_namespace AS ns ON ns.oid = cs.connamespace
	  LEFT JOIN pg_class AS tb ON cs.conrelid = tb.oid
	  WHERE ns.nspname= ] '@{schema}'

	%if @{table} %then
	  [ AND tb.relkind='r' AND tb.relname= ] '@{table}'
	%end
     %end

     %if @{last-sys-oid} %then
	%if @{schema} %then
	  [ AND ]
	%else
	  [ WHERE ]
	%end
	[ cs.oid > ] @{last-sys-oid}
     %end

     %if @{filter-oids} %then
       %if @{schema} %or @{last-sys-oid} %then
	 [ AND ]
       %else
	 [ WHERE ]
       %end

       [ cs.oid IN (] @{filter-oids} )
     %end

    %end
%end
