# Catalog queries for
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT pr.oid, proname AS name FROM pg_proc AS pr ]

  %if @{schema} %then
   [ LEFT JOIN pg_namespace AS ns ON pr.pronamespace = ns.oid
      WHERE pr.proisagg IS FALSE AND ns.nspname = ] '@{schema}'
  %else
   [ WHERE pr.proisagg IS FALSE ]
  %end

%else
    %if @{attribs} %then
	[SELECT pr.oid,
		pronamespace AS schema,
		pr.proowner AS owner,
		pr.proacl AS permissions,
		pr.proname AS name,
		pr.prolang AS language,
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

	(@{comment}) [ AS comment, ]
	(@{from-extension}) [ AS from_extension_bool ]

	[ FROM pg_proc AS pr ]

	%if @{schema} %then
	 [ LEFT JOIN pg_namespace AS ns ON pr.pronamespace = ns.oid ]
	%end

	[ WHERE pr.proisagg IS FALSE ]

	%if @{filter-oids} %or @{schema} %then
	  %if @{filter-oids} %then
	   [ AND pr.oid IN (] @{filter-oids} )
	  %end

	  %if @{schema} %then
	   [ AND  ns.nspname = ] '@{schema}'
	  %end
       %end
    %end
%end
