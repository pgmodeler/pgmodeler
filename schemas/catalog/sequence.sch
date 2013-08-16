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

  %if @{from-extension} %then
    [ AND ] (  @{from-extension} ) [ IS FALSE ]
  %end

%else
    %if @{attribs} %then
      [SELECT DISTINCT sq.oid, sq.relname AS name, sq.relnamespace AS schema, sq.relowner AS owner,
	      sq.relacl AS permissions,  ]

      #TODO: Discover which field is the start value for sequences on PgSQL 9.0
      %if @{pgsql90} %then
       [ 1 AS start, ]
      %else
       [ _sq1.start_value AS start, ]
      %end

      [ _sq1.minimum_value AS min_value,
	      _sq1.maximum_value AS max_value, _sq1.increment,

	  CASE _sq1.cycle_option
	    WHEN 'YES' THEN TRUE
	    ELSE FALSE
	  END AS cycle_bool, ]

      (@{comment}) [ AS comment ]

      [ FROM pg_class AS sq
	LEFT JOIN information_schema.sequences AS _sq1
	ON _sq1.sequence_schema=(SELECT nspname FROM pg_namespace WHERE oid=sq.relnamespace)
	WHERE relkind='S' ]

	%if @{last-sys-oid} %then
	  [ AND sq.oid ] @{oid-filter-op} $sp @{last-sys-oid}
	%end

	%if @{from-extension} %then
	  [ AND ] ( @{from-extension} ) [ IS FALSE ]
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
	   [ _sq1.sequence_schema= ] '@{schema}'
	  %end
       %end
    %end
%end
