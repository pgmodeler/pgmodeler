# Catalog queries for database
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT oid, datname AS name FROM pg_database WHERE datistemplate = FALSE]
%else
    %if @{attribs} %then
      [SELECT oid, datname AS name, pg_encoding_to_char(encoding) AS encoding, datdba AS owner,
	      datcollate AS lc_collate, datctype AS lc_ctype, datconnlimit AS connlimit,
	      datacl AS permissions, dattablespace AS tablespace, datdba AS owner, ]
	      (@{comment}) [ AS comment ]
      [ FROM pg_database WHERE datistemplate = FALSE ]

      %if @{filter-oids} %then
	[ AND oid IN (] @{filter-oids} )
      %end

    %end
%end
