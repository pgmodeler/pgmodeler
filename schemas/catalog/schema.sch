# Catalog queries for schemas
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT nspname FROM pg_namespace ]
  [ WHERE nspname <> 'information_schema' AND nspname NOT LIKE  'pg_%']
%else
    %if @{attribs} %then
      [SELECT ns.nspname AS name, sd.description FROM pg_namespace AS ns ]
      [  LEFT JOIN pg_description AS sd ON sd.objoid = ns.oid WHERE ns.nspname=] '@{name}'
    %end
%end
