# Catalog queries for foreign tables
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {list} %then
	%if {use-signature} %then
		%set {signature} [ ns.nspname || '.' || ]
	%end

	[ SELECT ft.oid, ft.relname AS name, ns.nspname AS parent, 
	  'schema' AS parent_type, NULL AS extra_info
	FROM pg_class AS ft
	LEFT JOIN pg_namespace AS ns ON ns.oid=ft.relnamespace ]

	%if {schema} %then
		[ WHERE ft.relkind = 'f' AND ns.nspname= ] '{schema}'
	%else
		[ WHERE ft.relkind = 'f' ]
	%end

	%if {last-sys-oid} %then
		[ AND ft.oid ] {oid-filter-op} $sp {last-sys-oid}
	%end

	%if {not-ext-object} %then
		[ AND ] ( {not-ext-object} )
	%end

	%if {name-filter} %then
		[ AND ] ( {signature} [ ft.relname ~* ] E'{name-filter}' )
	%end
%else
	%if {attribs} %then
		[SELECT ft.oid, ft.relname AS name, ft.relnamespace AS schema, ft.relowner AS owner, ft.relacl AS permission,
		ftserver AS server, ftoptions AS options, ]

		%if ({pgsql-ver} >=f "10.0") %then
			[ (SELECT array_agg(inhparent) AS parents FROM pg_inherits WHERE inhrelid = ft.oid
			 AND inhparent NOT IN (SELECT partrelid FROM pg_partitioned_table)),

			CASE relispartition
				WHEN TRUE THEN
				(SELECT inhparent FROM pg_inherits WHERE inhrelid = ft.oid)
				ELSE
				NULL
			END AS partitioned_table,

			pg_get_expr(relpartbound, ft.oid) AS partition_bound_expr, ]
		%else
			[ NULL AS partitioned_table, NULL AS partition_bound_expr, ]
		%end

		({comment}) [ AS comment ]

		[ FROM pg_class AS ft
		LEFT JOIN pg_foreign_table AS _ft1 ON _ft1.ftrelid=ft.oid ]

		%if ({pgsql-ver} >=f "10.0") %then
			[ LEFT JOIN pg_partitioned_table AS pt ON pt.partrelid = ft.oid ]
		%end

		[ WHERE ft.relkind = 'f' ]

		%if {last-sys-oid} %then
			[ AND ft.oid ] {oid-filter-op} $sp {last-sys-oid}
		%end

		%if {not-ext-object} %then
			[ AND ] ( {not-ext-object} )
		%end

		%if {filter-oids} %or {schema} %then
			[ AND ]

			%if {filter-oids} %then
				[ ft.oid IN (] {filter-oids} )

				%if {schema} %then
					[ AND ]
				%end
			%end
		%end
	%end
%end
