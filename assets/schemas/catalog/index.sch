# Catalog queries for indexes
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {list} %then
	%set {parent-name} [ ns.nspname || '.' || tb.relname ]

	%if {use-signature} %then
		%set {signature} {parent-name} [ || '.' || ]
	%end

	[SELECT id.indexrelid AS oid, cl.relname AS name, ] {parent-name} [ AS parent,
     'table' AS parent_type, NULL AS extra_info
	FROM pg_index AS id
	LEFT JOIN pg_class AS cl ON cl.oid = id.indexrelid
	LEFT JOIN pg_class AS tb ON id.indrelid = tb.oid
	LEFT JOIN pg_namespace AS ns ON ns.oid = tb.relnamespace ]

	%if {schema} %then
		[ WHERE nspname= ] '{schema}'

		%if {table} %then
			[ AND ((tb.relkind = 'r' OR tb.relkind = 'm' OR tb.relkind = 'p') AND tb.relname = ] '{table}' [)]
		%end
	%end

	%if {last-sys-oid} %then
		%if {schema} %then
			[ AND ]
		%else
			[ WHERE ]
		%end

		[ id.indexrelid ] {oid-filter-op} $sp {last-sys-oid}
	%end

	%if %not {schema} %and %not {last-sys-oid} %then
		[ WHERE ]
	%else
		[ AND ]
	%end

	%if ({pgsql-ver} >=f "10.0") %then
		# cl.relispartition IS FALSE avoids retriving indexes created automatically in partition tables
		[ cl.relispartition IS FALSE
		  AND (id.indisprimary IS FALSE AND id.indisexclusion IS FALSE) 
    		  AND ((SELECT count(oid) FROM pg_constraint WHERE conindid=id.indexrelid)=0) ]
	%else
		# This is just a place holder for the above clause 
		# because in version below 10 there is no relispartition field	
		[ TRUE ]
	%end

	%if {not-ext-object} %then
		[ AND ]( {not-ext-object} )
	%end

	%if {name-filter} %then
		[ AND ] ( {signature} [ cl.relname ~* ] E'{name-filter}' )
	%end

	%if {extra-condition} %then
		[ AND ] ( {extra-condition} )
	%end
%else
	%if {attribs} %then
		[SELECT id.indexrelid AS oid, cl.relname AS name,
		am.amname AS index_type, id.indrelid AS table, 
		id.indoption::int2] $ob $cb [ AS options, id.indisunique AS unique_bool, 
	    indcollation::oid] $ob $cb [ AS collations, 
		id.indkey::oid] $ob $cb [ AS columns,
		id.indclass::oid] $ob $cb [ AS opclasses,
		pg_get_expr(indexprs, indrelid) AS expressions,
		pg_get_expr(indpred, indrelid, true) predicate, ]

		%if ({pgsql-ver} >=f "11.0") %then
			[ id.indnkeyatts AS elements_count, ]
		%else
			[ id.indnatts AS elements_count, ]
		%end

		%if ({pgsql-ver} >=f "15.0") %then
			[ id.indnullsnotdistinct AS nulls_not_distinct_bool, ]
		%else
			[ FALSE AS nulls_not_distinct_bool, ]
		%end

		({comment}) [ AS comment ]

		[
		FROM pg_index AS id
		LEFT JOIN pg_class AS cl ON cl.oid = id.indexrelid
		LEFT JOIN pg_am AS am ON cl.relam = am.oid
		]

		%if {schema} %then
			[ LEFT JOIN pg_class AS tb ON id.indrelid = tb.oid
			LEFT JOIN pg_namespace AS ns ON ns.oid = tb.relnamespace
			WHERE ns.nspname= ] '{schema}'

			%if {table} %then
				[ AND ((tb.relkind = 'r' OR tb.relkind = 'm' OR tb.relkind = 'p') AND tb.relname = ] '{table}' [)]
			%end
		%end

		%if {last-sys-oid} %then
			%if {schema} %then
				[ AND ]
			%else
				[ WHERE ]
			%end

			[ id.indexrelid ] {oid-filter-op} $sp {last-sys-oid}
		%end

		%if {filter-oids} %then
			%if {schema} %or {last-sys-oid} %then
				[ AND ]
			%else
				[ WHERE ]
			%end

			[ id.indexrelid IN (] {filter-oids} )
		%end

		%if %not {schema} %and %not {last-sys-oid} %and %not {filter-oids} %then
			[ WHERE ]
		%else
			[ AND ]
		%end

		%if ({pgsql-ver} >=f "10.0") %then
			# cl.relispartition IS FALSE avoids retriving indexes created automatically in partition tables
			[ cl.relispartition IS FALSE
			  AND (id.indisprimary IS FALSE AND id.indisexclusion IS FALSE ) 
			  AND ((SELECT count(oid) FROM pg_constraint WHERE conindid=id.indexrelid)=0) ]
		%else
			# This is just a place holder for the above clause 
			# because in version below 10 there is no relispartition field	
			[ TRUE ]
		%end

		%if {not-ext-object} %then
			[ AND ]( {not-ext-object} )
		%end
	%end
%end
