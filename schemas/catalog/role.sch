# Catalog queries for roles
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT rolname AS name FROM pg_roles]
%else
    %if @{attribs} %then
	%if @{member-roles} %then
	  [SELECT rl1.rolname AS name, am.admin_option
	    FROM pg_auth_members AS am
	    LEFT JOIN pg_roles AS rl ON am.member=rl.oid
	    LEFT JOIN pg_roles AS rl1 ON am.roleid=rl1.oid
	    WHERE rl.rolname=] '@{name}'
	%else
	  [SELECT (SELECT passwd FROM pg_shadow WHERE usename= ] '@{name}' [), ]
	  [      rl.oid, rl.rolname AS name, rl.rolsuper AS superuser_bool, rl.rolinherit AS inherit_bool,
		 rl.rolcreaterole AS createrole_bool, rl.rolcreatedb AS createdb_bool, rl.rolcanlogin AS login_bool,
		 rl.rolconnlimit AS connlimit, rl.rolvaliduntil AS validity, sd.description AS comment
	    FROM pg_roles AS rl
	    LEFT JOIN pg_shdescription AS sd ON sd.objoid = rl.oid
	    WHERE rolname = ] '@{name}'
	%end
    %end
%end
