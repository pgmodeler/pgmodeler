# Catalog queries for tables
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [ SELECT tb.oid, tb.relname AS name FROM pg_class AS tb]

  %if @{schema} %then
    [ LEFT JOIN pg_namespace AS ns ON ns.oid=tb.relnamespace
      WHERE tb.relkind='r' AND ns.nspname= ] '@{schema}'
  %else
    [ WHERE tb.relkind='r']
  %end

  %if @{last-sys-oid} %then
     [ AND tb.oid ] @{oid-filter-op} $sp @{last-sys-oid}
  %end

  %if @{not-ext-object} %then
    [ AND ] ( @{not-ext-object} )
  %end

%else
    %if @{attribs} %then
    [SELECT tb.oid, tb.relname AS name, tb.relnamespace AS schema, tb.relowner AS owner,
	    tb.reltablespace AS tablespace, tb.relacl AS permission, relhasoids AS oids_bool, ]

    [(SELECT array_agg(inhparent) AS parents FROM pg_inherits WHERE inhrelid = tb.oid)],

    (@{comment}) [ AS comment ]

    [ FROM pg_class AS tb
      LEFT JOIN pg_tables AS _tb1 ON _tb1.tablename=tb.relname
      WHERE tb.relkind='r' ]

    %if @{last-sys-oid} %then
     [ AND tb.oid ] @{oid-filter-op} $sp @{last-sys-oid}
    %end

    %if @{not-ext-object} %then
     [ AND ] (  @{not-ext-object} )
    %end

      %if @{filter-oids} %or @{schema} %then
      [ AND ]
	%if @{filter-oids} %then
	 [ tb.oid IN (] @{filter-oids} )

	  %if @{schema} %then
	    [ AND ]
	  %end
	%end

	%if @{schema} %then
	 [ _tb1.schemaname= ] '@{schema}'
	%end
     %end

    %end
%end
