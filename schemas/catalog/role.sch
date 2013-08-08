# Catalog queries for roles
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT oid, rolname AS name FROM pg_roles]

  %if @{last-sys-oid} %then
   [ WHERE oid > ] @{last-sys-oid}
  %end

%else
    %if @{attribs} %then
	[ SELECT  rl1.oid, rolname AS name, rolsuper AS superuser_bool, rolinherit AS inherit_bool,
		  rolcreaterole AS createrole_bool, rolcreatedb AS createdb_bool, rolcanlogin AS login_bool,
		  rolconnlimit AS connlimit, rolvaliduntil AS validity,
	    (SELECT passwd FROM pg_shadow WHERE usesysid=oid),

	    (SELECT array_agg(rl.oid) AS members FROM pg_auth_members AS am
	     LEFT JOIN pg_roles AS rl ON rl.oid=am.member
	     WHERE am.roleid=rl1.oid AND am.admin_option IS FALSE),

	    (SELECT array_agg(rl.oid) AS admins FROM pg_auth_members AS am
	     LEFT JOIN pg_roles AS rl ON rl.oid=am.member
	     WHERE am.roleid=rl1.oid AND am.admin_option IS TRUE), ]

	    (@{comment}) [ AS comment

	    FROM pg_roles  AS rl1 ]

	    %if @{last-sys-oid} %then
	     [ WHERE oid > ] @{last-sys-oid}
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
