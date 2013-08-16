# Catalog queries for roles
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT oid, rolname AS name FROM pg_roles]

  %if @{last-sys-oid} %then
   [ WHERE oid ] @{oid-filter-op} $sp @{last-sys-oid}
  %end

%else
    %if @{attribs} %then
	[ SELECT  rl1.oid, rolname AS name, rolsuper AS superuser_bool, rolinherit AS inherit_bool,
		  rolcreaterole AS createrole_bool, rolcreatedb AS createdb_bool, rolcanlogin AS login_bool,
		  rolconnlimit AS connlimit, rolvaliduntil AS validity, TRUE AS encrypted_bool,  ]

	%if %not @{pgsql90} %then
	  [ rolreplication AS replication_bool, ]
	%else
	  [ NULL AS replication_bool, ]
	%end

	[   (SELECT passwd FROM pg_shadow WHERE usesysid=oid) AS password,

	    (SELECT array_agg(rl.oid) AS member_roles FROM pg_auth_members AS am
	     LEFT JOIN pg_roles AS rl ON rl.oid=am.member
	     WHERE am.roleid=rl1.oid AND am.admin_option IS FALSE),

	    (SELECT array_agg(rl.oid) AS admin_roles FROM pg_auth_members AS am
	     LEFT JOIN pg_roles AS rl ON rl.oid=am.member
	     WHERE am.roleid=rl1.oid AND am.admin_option IS TRUE),

	     NULL AS ref_roles, ]

	    (@{comment}) [ AS comment

	    FROM pg_roles  AS rl1 ]

	    %if @{last-sys-oid} %then
	     [ WHERE oid ] @{oid-filter-op} $sp @{last-sys-oid}
	    %end

	    %if @{filter-oids} %then
	      %if @{last-sys-oid} %then
		[ AND ]
	      %else
		[ WHERE ]
	      %end
	     [ oid IN (] @{filter-oids} )
	    %end
    %end
%end
