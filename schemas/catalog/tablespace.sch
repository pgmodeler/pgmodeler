# Catalog queries for tablespaces
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT spcname AS name FROM pg_tablespace]
%else
    %if @{attribs} %then
	[SELECT oid, spcname AS name, ]

	%if %not @{pgsql90} %and %not @{pgsql91} %then
	  [ pg_tablespace_location(oid) AS directory, ]
	%else
	  [ ts.spclocation AS directory ]
	%end

	[       spcowner AS owner, spcacl AS permissions
	  FROM pg_tablespace
	  WHERE spcname = ] '@{name}'
    %end
%end
