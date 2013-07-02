# Catalog queries for tablespaces
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT spcname AS name FROM pg_tablespace]
%else
    %if @{attribs} %then
	%if %not @{pgsql90} %and %not @{pgsql91} %then
	  [SELECT ts.spcname AS name, pg_tablespace_location(ts.oid) AS directory, ]
	  [       rl.rolname AS owner, sd.description AS comment, ts.spcacl AS permissions ]
	  [ FROM pg_tablespace AS ts ]
	  [ LEFT JOIN pg_roles AS rl ON rl.oid = ts.spcowner ]
	  [ LEFT JOIN pg_shdescription AS sd ON sd.objoid = ts.oid ]
	  [ WHERE ts.spcname = ] '@{name}'
	%else
	  [SELECT ts.spcname AS name, spclocation AS directory, rl.rolname AS owner, ]
	  [       sd.description AS comment, ts.spcacl AS permissions ]
	  [ FROM pg_tablespace AS ts ]
	  [ LEFT JOIN pg_roles AS rl ON rl.oid = ts.spcowner ]
	  [ LEFT JOIN pg_shdescription AS sd ON sd.objoid = ts.oid ]
	  [ WHERE ts.spcname = ] '@{name}'
	%end
    %end
%end
