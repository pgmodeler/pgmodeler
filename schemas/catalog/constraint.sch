# Catalog queries for constraints
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
[ SELECT cs.oid, cs.conname AS name FROM pg_constraint AS cs
   LEFT JOIN pg_namespace AS ns ON ns.oid = cs.connamespace
   LEFT JOIN pg_class AS tb ON cs.conrelid = tb.oid
   WHERE relkind='r' AND relname=] '@{table}' [ AND nspname= ] '@{schema}'
%else
    %if @{attribs} %then
     [SELECT cs.oid, cs.conname AS name, cs.conrelid AS table, ds.description AS comment,
	     cs.conkey AS src_columns, cs.confkey AS dst_columns,
	     cs.connoinherit AS no_inherit_bool, cs.consrc AS expression,
	     cs.condeferrable AS deferrable_bool, cs.confrelid AS ref_table,
	     cl.reltablespace AS tablespace, cs.conexclop AS operators,
	     cl.relam AS index_type,

	CASE cs.contype
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
     LEFT JOIN pg_namespace AS ns ON ns.oid = cs.connamespace
     LEFT JOIN pg_description AS ds ON ds.objoid=cs.oid
     LEFT JOIN pg_class AS tb ON cs.conrelid = tb.oid
     LEFT JOIN pg_class AS cl ON cl.oid = cs.conindid
     WHERE tb.relkind='r' AND tb.relname= ] '@{table}' [ AND ns.nspname= ] '@{schema}'
    %end
%end
