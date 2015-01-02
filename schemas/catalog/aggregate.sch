# Catalog queries for aggregates
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT pr.oid, proname || '(' || array_to_string(proargtypes::regtype] $ob $cb [,',') || ')'  AS name FROM pg_proc AS pr ]

  %if @{schema} %then
    [ LEFT JOIN pg_namespace AS ns ON pr.pronamespace = ns.oid
       WHERE pr.proisagg IS TRUE AND ns.nspname = ] '@{schema}'
  %else
    [ WHERE pr.proisagg IS TRUE ]
  %end

  %if @{last-sys-oid} %then
   [ AND pr.oid ] @{oid-filter-op} $sp @{last-sys-oid}
  %end

  %if @{not-ext-object} %then
    [ AND ] ( @{not-ext-object} ) # [ IS FALSE ]
  %end

%else
    %if @{attribs} %then
      [SELECT pr.oid AS oid, ag.aggfnoid AS name, ag.aggtransfn::oid AS transition,
	      ag.aggfinalfn::oid AS final, ag.aggsortop::oid AS sort_op, aggtranstype AS state_type,
	      pr.proargtypes::oid] $ob $cb [ AS types, ag.agginitval AS initial_cond, pr.proowner AS owner,
	      pr.pronamespace AS schema, pr.proacl AS permission, ]

       (@{comment}) [ AS comment ]

       [ FROM pg_aggregate AS ag
	 LEFT JOIN pg_proc AS pr ON pr.oid=ag.aggfnoid::oid ]

      %if @{schema} %then
       [ LEFT JOIN pg_namespace AS ns ON pr.pronamespace = ns.oid ]
      %end

      [ WHERE pr.proisagg IS TRUE ]
      %if @{last-sys-oid} %then
	[ AND pr.oid ] @{oid-filter-op} $sp @{last-sys-oid}
      %end

      %if @{not-ext-object} %then
	[ AND ] ( @{not-ext-object} )
      %end

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
