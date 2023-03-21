# Catalog queries for database
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {list} %then
	[SELECT oid, datname AS name, ]

	# According to PostgreSQL sources the maximum OID value for a built-in object (A.K.A system object) is 16383.
	# This way we query the pg_depend and get the latest object id closest to that limit (16383) and assume it
	# as the last built-in system object OID. 
	# Reference: https://github.com/postgres/postgres/blob/master/src/include/access/transam.h#L156)

	[ (select objid as last_system_oid from pg_depend
		where objid > 0 and objid <= 16383
		order by objid desc limit 1) AS last_sys_oid ]

	[ FROM pg_database
	  WHERE datistemplate = FALSE ]

	%if {name} %then
		[ AND datname = ] '{name}'
	%end
%else
	%if {attribs} %then
		[SELECT oid, datname AS name, pg_encoding_to_char(encoding) AS encoding, datdba AS owner,
		datcollate AS lc_collate, datctype AS lc_ctype, datconnlimit AS connlimit,
		datacl AS permission, dattablespace AS tablespace, datdba AS owner,
		NULL AS template, ]

		({comment}) [ AS comment, datallowconn AS allow_conns_bool, datistemplate AS is_template_bool 
		FROM pg_database WHERE datistemplate = FALSE ]

		#%if {last-sys-oid} %then
		#	[ AND oid ] {oid-filter-op} $sp {last-sys-oid}
		#%end

		%if {filter-oids} %then
			[ AND oid IN (] {filter-oids} )
		%end

		%if {name} %then
			[ AND datname = ] '{name}'
		%end
	%end
%end
