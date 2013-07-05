# Catalog queries for
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT pr.oid, proname AS name FROM pg_proc AS pr ]

  %if @{schema} %then
   [ LEFT JOIN pg_namespace AS ns ON pr.pronamespace = ns.oid
      WHERE ns.nspname = ] '@{schema}'
  %end

%else
    %if @{attribs} %then
	[SELECT pr.oid,
		ns.nspname AS schema,
		pr.proowner AS owner,
		pr.proacl AS permissions,
		pr.proname AS name,
		la.lanname AS language,
		pr.procost AS execution_cost,
		pr.prorows AS row_amount,
		pr.proiswindow AS window_func_bool,
		pr.proretset AS returns_setof_bool,
		pr.pronargs AS arg_count,
		pr.pronargdefaults AS arg_def_count,
		pr.prorettype AS return_type,
		proargtypes::oid ] $ob $cb

	[ AS arg_types,
		proallargtypes AS all_arg_types,
		pr.proargmodes AS arg_modes,
		pr.proargnames AS arg_names,
		pr.proargdefaults AS arg_defaults,
		pr.prosrc AS definition,
		pr.probin AS library,

		CASE
		   WHEN pr.prosecdef  THEN 'SECURITY DEFINER'
		   ELSE 'SECURITY INVOKER'
		END AS security_type,

		CASE
		   WHEN pr.provolatile='v' THEN 'VOLATILE'
		   WHEN pr.provolatile='i' THEN 'IMMUTABLE'
		   ELSE 'STABLE'
		END AS function_type,

		CASE
		   WHEN pr.proisstrict THEN 'STRICT'
		   ELSE 'CALLED ON NULL INPUT'
		END AS behavior_type, ]

		%if @{pgsql90} %or @{pgsql91} %then
		 [ NULL AS leakproof_bool, ]
		%else
		 [ pr.proleakproof AS leakproof_bool, ]
		%end

	(@{owner}) [ AS owner, ]
	(@{comment}) [ AS comment, ]
	(@{from-extension}) [ AS from_extension_bool ]

	[ FROM pg_proc AS pr
	    LEFT JOIN pg_language AS la ON pr.prolang = la.oid
	    LEFT JOIN pg_namespace AS ns ON pr.pronamespace = ns.oid ]

       %if @{filter-oids} %or @{schema} %then
	 [ WHERE ]

	 %if @{filter-oids} %then
	   [ pr.oid IN (] @{filter-oids} )

	   %if @{schema} %then
	     [ AND ]
	   %end
	 %end

	 %if @{schema} %then
	   [ ns.nspname = ] '@{schema}'
	 %end
       %end

    %end
%end
