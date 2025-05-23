# Catalog queries for functions
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} <=f "10.0") %then
	%set {is-not-agg-proc} [pr.proisagg IS FALSE]
	%set {window-func} [pr.proiswindow AS window_func_bool]
%else
	%set {is-not-agg-proc} [pr.prokind NOT IN ('a', 'p') ]
	%set {window-func} [CASE pr.prokind WHEN 'w' THEN TRUE ELSE FALSE END AS window_func_bool]
%end

%if {list} %then
	%if {use-signature} %then
		%set {signature} [ ns.nspname || '.' || ]
	%end

	[SELECT pr.oid, pr.proname || '(' ||
		CASE
		WHEN array_length(pr.proargtypes, 1) = 0 THEN ''
		ELSE
		  array_to_string(
			(SELECT array_agg(
				CASE ns.nspname
				WHEN 'pg_catalog' then ''
				ELSE ns.nspname || '.' 
				END || tp.typname )
			 FROM (SELECT unnest(proargtypes) AS oid FROM pg_proc WHERE oid = pr.oid) AS atp
			 LEFT JOIN pg_type AS tp ON tp.oid = atp.oid
			 LEFT JOIN pg_namespace AS ns ON ns.oid = tp.typnamespace),',')
		END || ')' AS name,
	    ns.nspname AS parent, 'schema' AS parent_type, NULL AS extra_info	
	 FROM pg_proc AS pr
 	 LEFT JOIN pg_namespace AS ns ON pr.pronamespace = ns.oid ]

	%if {schema} %then
		[ WHERE ] {is-not-agg-proc} [ AND ns.nspname = ] '{schema}'
	%else
		[ WHERE ] {is-not-agg-proc}
	%end

	%if {last-sys-oid} %then
		[ AND pr.oid ] {oid-filter-op} $sp {last-sys-oid}
	%end

	%if {not-ext-object} %then
		[ AND ] ( {not-ext-object} )
	%end

	%if {name-filter} %then
		[ AND ] ( {signature} [ pr.proname ~* ] E'{name-filter}' )
	%end
%else
	%if {attribs} %then
		[SELECT pr.oid,
		pronamespace AS schema,
		pr.proowner AS owner,
		pr.proacl AS permission,
		pr.proname AS name,
		pr.prolang AS language,
		pr.procost AS execution_cost,
		pr.prorows AS row_amount,
		array_to_string(pr.proconfig, '] $ds [', '') AS config_params, ]

		{window-func},

		[ pr.proretset AS returns_setof_bool,
		pr.pronargs AS arg_count,
		pr.pronargdefaults AS arg_def_count,
		pr.prorettype AS return_type, ]

		[ CASE
		WHEN proallargtypes IS NOT NULL THEN proallargtypes
		ELSE proargtypes::oid ] $ob $cb
		[ END AS arg_types,

		pr.proargmodes AS arg_modes,
		pr.proargnames AS arg_names,
		pg_get_expr(proargdefaults, 'pg_class'::regclass) AS arg_defaults,
		pr.prosrc AS definition,
		pr.probin AS library,

		CASE
		WHEN pr.prosecdef THEN 'SECURITY DEFINER'
		ELSE 'SECURITY INVOKER'
		END AS security_type,

		CASE
		WHEN pr.provolatile='v' THEN 'VOLATILE'
		WHEN pr.provolatile='i' THEN 'IMMUTABLE'
		ELSE 'STABLE'
		END AS function_type,

		CASE
			WHEN pr.proisstrict THEN 'RETURNS NULL ON NULL INPUT'
			ELSE 'CALLED ON NULL INPUT'
		END AS behavior_type, 

		pr.proleakproof AS leakproof_bool, 
	
		CASE
			WHEN pr.proparallel='u' THEN 'PARALLEL UNSAFE'
			WHEN pr.proparallel='r' THEN 'PARALLEL RESTRICTED'
			ELSE 'PARALLEL SAFE'
		END AS parallel_type,

		pr.protrftypes AS transform_types, ]
	
		({comment}) [ AS comment ]

		[ FROM pg_proc AS pr ]

		%if {schema} %then
			[ LEFT JOIN pg_namespace AS ns ON pr.pronamespace = ns.oid ]
		%end

		[ WHERE ] {is-not-agg-proc}

		%if {last-sys-oid} %then
			[ AND pr.oid ] {oid-filter-op} $sp {last-sys-oid}
		%end

		%if {not-ext-object} %then
			[ AND ] ( {not-ext-object} )
		%end

		%if {filter-oids} %or {schema} %then
			%if {filter-oids} %then
				[ AND pr.oid IN (] {filter-oids} )
			%end

			%if {schema} %then
				[ AND ns.nspname = ] '{schema}'
			%end
		%end
	%end
%end
