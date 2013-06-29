# Catalog queries for database
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT datname AS name FROM pg_database WHERE datistemplate = FALSE]
%else
  %if @{count} %then
    [SELECT count(datname) AS count FROM pg_database WHERE datistemplate = FALSE]
  %else
    %if @{attribs} %then
      [SELECT db.datname AS name, pg_encoding_to_char(db.encoding) AS encoding, rl.rolname AS owner, ]
      [       db.datcollate AS lc_collate, db.datctype AS lc_ctype, db.datconnlimit AS connlimit, ]
      [       ts.spcname AS tablespace, sd.description AS comment ]
      [ FROM pg_database AS db ]
      [ LEFT JOIN pg_tablespace AS ts ON ts.oid = db.dattablespace ]
      [ LEFT JOIN pg_description AS ds ON ds.objoid = db.oid ]
      [ LEFT JOIN pg_roles AS rl ON rl.oid = db.datdba ]
      [ LEFT JOIN pg_shdescription AS sd ON sd.objoid = db.oid ]
      [ WHERE db.datname = ] '@{name}'
    %end
  %end
%end
