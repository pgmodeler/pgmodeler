# Catalog queries for tablespaces
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {list} %then
	[SELECT oid, spcname AS name, current_database() AS parent, 'database' AS parent_type, NULL AS extra_info
	FROM pg_tablespace ]

	%if {last-sys-oid} %then
		[ WHERE oid ] {oid-filter-op} $sp {last-sys-oid}
	%end

	%if {name-filter} %then
		%if {last-sys-oid} %then
			[ AND ]
		%else
			[ WHERE ]
		%end

		( [spcname ~* ] E'{name-filter}' )
	%end
%else
	%if {attribs} %then
		[SELECT oid, spcname AS name, spcacl AS permission, spcowner AS owner, 
		 pg_tablespace_location(oid) AS directory, ]

		({comment}) [ AS comment ]

		[ FROM pg_tablespace ]

		%if {last-sys-oid} %then
			[ WHERE oid ] {oid-filter-op} $sp {last-sys-oid}
		%end

		%if {filter-oids} %then
			%if {last-sys-oid} %then
				[ AND ]
			%else
				[ WHERE ]
			%end

			[ oid IN (] {filter-oids} )
		%end
	%end
%end
