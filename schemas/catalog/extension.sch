# Catalog queries for extensions
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT extname AS name FROM pg_extension AS ex
    LEFT JOIN pg_namespace AS ns ON ex.extnamespace = ns.oid
    WHERE ns.nspname = ] '@{schema}'
%else
    %if @{attribs} %then
	%if @{handles-type} %then
	  [SELECT objid FROM pg_depend AS dp
	    LEFT JOIN pg_extension AS ex ON ex.oid=dp.objid
	    LEFT JOIN pg_namespace AS ns ON ns.oid=dp.refobjid
	    WHERE  dp.objid::regtype::text = ] '@{name}'
	  [  AND dp.classid::regclass::text = 'pg_type'
	     AND ns.nspname = ] '@{schema}'
	%else
	  [SELECT ex.oid, ex.extname AS name, ex.extversion AS curr_version, ns.nspname AS schema
	   FROM pg_extension AS ex
	   LEFT JOIN pg_namespace AS ns ON ex.extnamespace = ns.oid
	   WHERE ex.extname = ] '@{name}' [ AND ns.nspname = ] '@{schema}'
	%end
    %end
%end
