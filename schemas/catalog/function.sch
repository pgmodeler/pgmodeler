# Catalog queries for
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} <=f "10.0") %then
    %set {is-not-agg} [pr.proisagg IS FALSE]
    %set {window-func} [pr.proiswindow AS window_func_bool]
%else
    %set {is-not-agg} [pr.prokind <> 'a'] 
    %set {window-func} [CASE pr.prokind WHEN 'w' THEN TRUE ELSE FALSE END AS window_func_bool]
%end    

%if {list} %then
  [SELECT pr.oid,  proname || '(' || array_to_string(proargtypes::regtype] $ob $cb [,',') || ')' AS name,
    ns.nspname AS parent,
    'schema' AS parent_type
    FROM pg_proc AS pr 
    LEFT JOIN pg_namespace AS ns ON pr.pronamespace = ns.oid ]

  %if {schema} %then
   [ WHERE ] {is-not-agg} [ AND ns.nspname = ] '{schema}'
  %else
   [ WHERE ] {is-not-agg}
  %end

  %if {last-sys-oid} %then
    [ AND pr.oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if {not-ext-object} %then
    [ AND ] ( {not-ext-object} )
  %end
  
  %if {name-filter} %then
    [ AND ] ( {name-filter} )
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
		pr.prorows AS row_amount, ]
		
		{window-func},
		
	[	pr.proretset AS returns_setof_bool,
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

        %if ({pgsql-ver} <=f "9.1") %then
		 [ NULL AS leakproof_bool, ]
		%else
		 [ pr.proleakproof AS leakproof_bool, ]
		%end

	({comment}) [ AS comment ]

	[ FROM pg_proc AS pr ]

	%if {schema} %then
	 [ LEFT JOIN pg_namespace AS ns ON pr.pronamespace = ns.oid ]
	%end

	[ WHERE ] {is-not-agg} 

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
	   [ AND  ns.nspname = ] '{schema}'
	  %end
       %end
    %end
%end
