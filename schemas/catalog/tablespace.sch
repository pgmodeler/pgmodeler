# Catalog queries for tablespaces
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT oid, spcname AS name FROM pg_tablespace
    WHERE spcname LIKE 'pg_%']
%else
    %if @{attribs} %then
	[SELECT oid, spcname AS name, spcacl AS permissions, spcowner AS owner, ]

	%if %not @{pgsql90} %and %not @{pgsql91} %then
	  [ pg_tablespace_location(oid) AS directory, ]
	%else
	  [ ts.spclocation AS directory ]
	%end

	(@{comment}) [ AS comment ]

	[ FROM pg_tablespace ]

	%if @{filter-oids} %then
	 [ WHERE oid IN (] @{filter-oids} )
	%end
    %end
%end
