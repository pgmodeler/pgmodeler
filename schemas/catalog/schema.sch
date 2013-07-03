# Catalog queries for schemas
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  # Schemas pg_catalog, pg_toast and information_schema aren't listed
  [SELECT nspname FROM pg_namespace
    WHERE nspname <> 'information_schema' AND nspname NOT LIKE  'pg_%']
%else
    %if @{attribs} %then
      [SELECT ns.oid, ns.nspname AS name, ns.nspacl AS permissions,
	     rl.rolname AS owner, ds.description AS comment
       FROM pg_namespace AS ns
       LEFT JOIN pg_roles AS rl ON rl.oid = ns.nspowner
       LEFT JOIN pg_description AS ds ON ds.objoid = ns.oid
       WHERE ns.nspname = ] '@{name}'
    %end
%end
