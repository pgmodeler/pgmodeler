# Catalog queries for indexes
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
[SELECT id.indexrelid AS oid, cl.relname AS name FROM pg_index AS id
  LEFT JOIN pg_class AS tb ON id.indrelid = tb.oid
  LEFT JOIN pg_class AS cl ON cl.oid = id.indexrelid
  LEFT JOIN pg_namespace AS ns ON ns.oid = tb.relnamespace
  WHERE tb.relkind = 'r' AND tb.relname = ] '@{table}' [ AND nspname= ] '@{schema}'

  %if @{last-sys-oid} %then
    [ AND id.indexrelid > ] @{last-sys-oid}
  %end

%else
    %if @{attribs} %then
      [SELECT id.indexrelid AS oid, cl.relname AS name,
	      cl.relam AS index_type, id.indrelid AS table,
	      id.indisprimary, id.indisunique AS unique_bool, ]

      %if @{pgsql90} %then
       [ NULL AS collations, ]
      %else
       [ indcollation::oid] $ob $cb [ AS collations, ]
      %end

      [       id.indkey::oid] $ob $cb [ AS columns,
	      id.indclass::oid] $ob $cb [ AS opclass,
	      string_to_array(pg_get_expr(indexprs, indrelid),',') AS elements,
	      pg_get_expr(indpred, indrelid, true) condition,
	      ds.description AS comment
	FROM pg_index AS id
	LEFT JOIN pg_class AS tb ON id.indrelid = tb.oid
	LEFT JOIN pg_class AS cl ON cl.oid = id.indexrelid
	LEFT JOIN pg_description ds ON ds.objoid = id.indexrelid
	LEFT JOIN pg_namespace AS ns ON ns.oid = tb.relnamespace
	WHERE tb.relkind = 'r' AND tb.relname = ] 'filo' [ AND nspname = ] 'public'

      %if @{last-sys-oid} %then
       [ AND id.indexrelid > ] @{last-sys-oid}
      %end

      %if @{filter-oids} %then
	[ AND id.indexrelid IN (] @{filter-oids} )
      %end
    %end
%end
