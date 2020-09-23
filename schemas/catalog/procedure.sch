# Catalog queries for procedures
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} >=f "11.0") %then

    %set {is-procedure} [pr.prokind = 'p'] 

    %if {list} %then
        %if {use-signature} %then
            %set {signature} [ ns.nspname || '.' || ]
        %end

        [ SELECT pr.oid, proname || '(' || array_to_string(proargtypes::regtype] $ob $cb [,',') || ')' AS name,
                 ns.nspname AS parent, 'schema' AS parent_type
          FROM pg_proc AS pr 
          LEFT JOIN pg_namespace AS ns ON pr.pronamespace = ns.oid ]

        %if {schema} %then
            [ WHERE ] {is-procedure} [ AND ns.nspname = ] '{schema}'
        %else
            [ WHERE ] {is-procedure}
        %end

        %if {last-sys-oid} %then
            [ AND pr.oid ] {oid-filter-op} $sp {last-sys-oid}
        %end

        %if {not-ext-object} %then
            [ AND ] ( {not-ext-object} )
        %end
    
        %if {name-filter} %then
            [ AND ] ( {signature} [ pr.proname ~* ] E'{name-filter}' )
        %end
    %else
        %if {attribs} %then
        
            [ SELECT pr.oid, pronamespace AS schema, pr.proowner AS owner,
                     pr.proacl AS permission, pr.proname AS name, pr.prolang AS language,
                     pr.pronargs AS arg_count, pr.pronargdefaults AS arg_def_count, ]

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
                  END AS security_type, ]

            ({comment}) [ AS comment ]

            [ FROM pg_proc AS pr ]

            %if {schema} %then
                [ LEFT JOIN pg_namespace AS ns ON pr.pronamespace = ns.oid ]
            %end

            [ WHERE ] {is-procedure} 

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
%end
