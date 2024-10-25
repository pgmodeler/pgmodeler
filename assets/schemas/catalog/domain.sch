# Catalog queries for domains
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {list} %then
	%if {use-signature} %then
		%set {signature} [ _dm1.domain_schema || '.' || ]
	%end

	[SELECT dm.oid, dm.typname AS name,
	_dm1.domain_schema AS parent, 'schema' AS parent_type, NULL AS extra_info
    FROM pg_type AS dm
	INNER JOIN information_schema.domains AS _dm1 ON dm.typname=_dm1.domain_name
	WHERE dm.typrelid=0 ]

	%if {schema} %then
		[ AND domain_schema=] '{schema}'
	%end

	%if {last-sys-oid} %then
		[ AND dm.oid ] {oid-filter-op} $sp {last-sys-oid}
	%end

	%if {not-ext-object} %then
		[ AND ]( {not-ext-object} )
	%end

	%if {name-filter} %then
		[ AND ] ( {signature} [ dm.typname ~* ] E'{name-filter}' )
	%end
%else
	%if {attribs} %then
		[SELECT dm.oid, dm.typname AS name, dm.typowner AS owner, dm.typnamespace AS schema, dm.typndims AS dimension,
		dm.typbasetype AS type, dm.typacl AS permission, dm.typcollation AS collation, 

		CASE
			WHEN _dm1.numeric_precision_radix IS NOT NULL THEN _dm1.numeric_precision_radix
			ELSE _dm1.character_maximum_length
		END AS length,

		CASE
			WHEN _dm1.numeric_precision_radix IS NOT NULL THEN _dm1.numeric_scale 
			WHEN _dm1.datetime_precision IS NOT NULL THEN _dm1.datetime_precision 
			WHEN _dm1.interval_precision IS NOT NULL THEN _dm1.interval_precision 
			ELSE NULL
		END AS precision,

		dm.typnotnull AS not_null_bool,
		_dm1.interval_type, _dm1.domain_default AS default_value,
		(select array_to_string(array_agg(conname || ' ' || pg_get_constraintdef(oid)), '] $ds [') 
         from pg_constraint where contype <> 'n' and contypid = dm.oid) as constraints,
		]

		({comment}) [ AS comment ]

		[ FROM pg_type AS dm
		LEFT JOIN information_schema.domains AS _dm1 ON dm.typname=_dm1.domain_name
		WHERE dm.typrelid=0 ]

		%if {filter-oids} %or {schema} %then
			[ AND ]

			%if {filter-oids} %then
				[ dm.oid IN (] {filter-oids} )

				%if {schema} %then
					[ AND ]
				%end
			%end

			%if {schema} %then
				[ _dm1.domain_schema= ] '{schema}'
			%end
		%end

		%if {last-sys-oid} %then
			[ AND dm.oid ] {oid-filter-op} $sp {last-sys-oid}
		%end

		%if {not-ext-object} %then
			[ AND ]( {not-ext-object} )
		%end
	%end
%end
