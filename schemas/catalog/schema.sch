# Catalog queries for schemas
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  # Schemas pg_catalog, pg_toast and information_schema aren't listed
  [SELECT nspname AS name FROM pg_namespace
    WHERE nspname <> 'information_schema' AND nspname NOT LIKE  'pg_%']
%else
    %if @{attribs} %then
      [SELECT oid, nspname AS name, nspacl AS permissions, nspowner AS owner
       FROM pg_namespace
       WHERE nspname = ] '@{name}'
    %end
%end
