# Catalog queries for sequences
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT sq.oid, relname AS name FROM pg_class AS sq ]

  %if @{schema} %then
    [ LEFT JOIN pg_namespace AS ns ON sq.relnamespace = ns.oid
       WHERE relkind='S' AND ns.nspname = ] '@{schema}'
  %else
    [ WHERE relkind='S']
  %end

  %if @{last-sys-oid} %then
     [ AND sq.oid ] @{oid-filter-op} $sp @{last-sys-oid}
  %end

  %if @{not-ext-object} %then
    [ AND ] (  @{not-ext-object} )
  %end

%else
    %if @{attribs} %then
      #[SELECT sq.oid, sq.relname AS name, sq.relnamespace AS schema,
      #  (SELECT refobjid || ':' || refobjsubid FROM pg_depend WHERE objid=sq.oid AND deptype='a') AS owner_col, ]

      #TODO: Discover which field is the start value for sequences on PgSQL 9.0
      #%if @{pgsql90} %then
      # [ 1 AS start,  sq.relacl::text AS permission,  ]
      #%else
      # [ _sq1.start_value AS start,  sq.relacl AS permission, ]
      #%end

      #[ 1 AS cache,
      #	_sq1.minimum_value AS min_value,
      #	_sq1.maximum_value AS max_value, _sq1.increment,

      #	  CASE _sq1.cycle_option
      #	    WHEN 'YES' THEN TRUE
      #	    ELSE FALSE
      #	  END AS cycle_bool, ]

      # (@{comment}) [ AS comment ]

      #[ FROM pg_class AS sq
      #	LEFT JOIN information_schema.sequences AS _sq1
      #	ON _sq1.sequence_schema=(SELECT nspname FROM pg_namespace WHERE oid=sq.relnamespace)
      #	  AND _sq1.sequence_name=sq.relname
      #	WHERE relkind='S' ]


      #Creates a temporary function to retrieve the sequence attributes
      [CREATE OR REPLACE FUNCTION pg_temp.get_seq_attribs(text,text)
       RETURNS text
       AS
       $$
	DECLARE
	 res record;
	BEGIN
	 EXECUTE 'SELECT start_value, min_value, max_value, increment_by, cache_value, ' ||
		 ' CASE WHEN is_cycled IS FALSE THEN NULL ' ||
		 ' ELSE  is_cycled END FROM ' || $1 || '.' || $2 || ' LIMIT 1;' INTO res;
	 RETURN replace(replace(res::text,'(','{'),')','}');
	END
       $$
       LANGUAGE plpgsql;]

      [ SELECT sq.oid, sq.relname AS name, sq.relnamespace AS schema, sq.relowner AS owner,
	 (SELECT refobjid || ':' || refobjsubid FROM pg_depend WHERE objid=sq.oid AND deptype='a') AS owner_col,
	  (pg_temp.get_seq_attribs(ns.nspname, sq.relname)) AS attribute, ]

      (@{comment}) [ AS comment ]

      [ FROM pg_class AS sq
	 LEFT JOIN pg_namespace AS ns ON ns.oid=sq.relnamespace
	 WHERE relkind='S' ]

	%if @{last-sys-oid} %then
	  [ AND sq.oid ] @{oid-filter-op} $sp @{last-sys-oid}
	%end

	%if @{not-ext-object} %then
	  [ AND ] ( @{not-ext-object} )
	%end

	%if @{filter-oids} %or @{schema} %then
	[ AND ]
	  %if @{filter-oids} %then
	   [ sq.oid IN (] @{filter-oids} )

	    %if @{schema} %then
	      [ AND ]
	    %end
	  %end

	  %if @{schema} %then
	   [ ns.nspname= ] '@{schema}'
	  %end
       %end
    %end
%end
