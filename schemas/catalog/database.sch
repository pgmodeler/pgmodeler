# Catalog queries for database
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT datname AS name FROM pg_database WHERE datistemplate = FALSE]
%else
    %if @{attribs} %then
      [SELECT oid, datname AS name, pg_encoding_to_char(encoding) AS encoding, datdba AS owner,
	      datcollate AS lc_collate, datctype AS lc_ctype, datconnlimit AS connlimit,
	      dattablespace AS tablespace, datacl AS permissions
	FROM pg_database
	WHERE datname = ] '@{name}'
    %end
%end
