# Catalog queries for schemas
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT oid, nspname AS name FROM pg_namespace ]
  #WHERE nspname <> 'information_schema' AND nspname NOT LIKE  'pg_%']
%else
    %if @{attribs} %then
      [SELECT oid, nspname AS name, nspacl AS permissions, nspowner AS owner, ]

	(@{comment}) [ AS comment, ]
	(@{from-extension}) [ AS from_extension_bool ]

       [ FROM pg_namespace ]

       %if @{filter-oids} %then
	 [ WHERE oid IN (] @{filter-oids} )
       %end

    %end
%end
