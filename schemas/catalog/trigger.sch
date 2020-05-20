# Catalog queries for trigger
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [SELECT tg.oid, tgname AS name, tb.oid::regclass::text AS parent, 'table' AS parent_type
    FROM pg_trigger AS tg
    LEFT JOIN pg_class AS tb ON tg.tgrelid = tb.oid AND relkind IN ('r','v','m','f') ]

  %if {schema} %then
    [  LEFT JOIN pg_namespace AS ns ON ns.oid = tb.relnamespace
       WHERE ns.nspname= ] '{schema}'

    %if {table} %then
     [ AND tb.relname=]'{table}'
    %end

   [ AND ]
  %else
   [ WHERE ]
  %end

  [ tgisinternal IS FALSE ]

  %if {last-sys-oid} %then
    [ AND tg.oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if {not-ext-object} %then
    [ AND ]( {not-ext-object} )
  %end
  
  %if {name-filter} %then
    [ AND ] ( {name-filter} )
  %end
  
%else
    %if {attribs} %then
    # pg_trigger.tgtype datails:
    #   bit 0 -> FOR EACH ROW [1] | STATEMENT [0]
    #   bit 1 -> AFTER [0] | BEFORE [1]
    #   bit 2 -> INSERT
    #   bit 3 -> DELETE
    #   bit 4 -> UPDATE
    #   bit 5 -> TRUNCATE
    #   bit 6 -> INTEAD OF [1] (only for views)
      [ SELECT DISTINCT(tg.oid), tg.tgname AS name, tg.tgfoid AS trigger_func, tb.oid AS table,
      
        CASE 
            WHEN tb.relkind = 'r' THEN 'table'
            WHEN tb.relkind = 'f' THEN 'foreigntable'
            WHEN tb.relkind = 'v' THEN 'view'
            WHEN tb.relkind = 'm' THEN 'view'
        END AS table_type, 

	#Convert the arguments from bytea to a string array. The last element is always empty and can be discarded
	string_to_array(encode(tg.tgargs,'escape'), E'\\000') AS arguments,

	       it.action_condition AS condition,
	       (B'0000001'::integer & tgtype = 1) AS per_line_bool,
	       (B'0000100'::integer & tgtype = 4) AS ins_event_bool,
	       (B'0001000'::integer & tgtype = 8) AS del_event_bool,
	       (B'0010000'::integer & tgtype = 16) AS upd_event_bool,
	       (B'0100000'::integer & tgtype = 32) AS trunc_event_bool,
	       tg.tgdeferrable AS deferrable_bool,
	       (tg.tgconstraint > 0) AS constraint_bool,

	    CASE
	      WHEN B'0000010'::integer & tgtype = 2 THEN 'BEFORE'
	      WHEN B'1000000'::integer & tgtype = 64 THEN 'INSTEAD OF'
	    ELSE 'AFTER'
	    END AS firing_type,

	    CASE tg.tginitdeferred
	      WHEN TRUE THEN 'INITIALLY DEFERRED'
	      ELSE 'INITIALLY IMMEDIATE'
	    END AS defer_type,

	    CASE
	      WHEN (tg.tgconstraint > 0) THEN
                regexp_replace(regexp_replace(pg_get_triggerdef(tg.oid), '(.)+((INSERT|DELETE|UPDATE)|( OF))', ''), '( ON)(.)*','')
	      ELSE NULL
	    END AS columns, ]
	    
	    
     %if ({pgsql-ver} >=f "10.0") %then
        [ tgoldtable AS old_table_name, tgnewtable AS new_table_name, ]
     %else
        [ NULL AS old_table_name, NULL AS new_table_name, ]
     %end

         ({comment}) [ AS comment ] 
	    
	 [ FROM pg_trigger AS tg
	 LEFT JOIN pg_constraint AS cs ON tg.tgconstraint = cs.oid
	 LEFT JOIN pg_class AS tb ON tg.tgrelid = tb.oid
	 LEFT JOIN pg_namespace AS ns ON ns.oid = tb.relnamespace
	 LEFT JOIN information_schema.triggers AS it ON
		   it.trigger_schema=ns.nspname AND
		   it.trigger_name=tg.tgname AND
		   it.event_object_table=tb.relname
	 WHERE tg.tgisinternal IS FALSE ]

      %if {schema} %then
       [  AND ns.nspname= ] '{schema}'

	%if {table} %then
	  [ AND tb.relname=]'{table}'
	%end
      %end

       %if {last-sys-oid} %then
	 [ AND tg.oid ] {oid-filter-op} $sp {last-sys-oid}
       %end

	%if {filter-oids} %then
	  [ AND tg.oid IN (] {filter-oids} )
	%end

        %if {not-ext-object} %then
          [ AND ]( {not-ext-object} )
        %end
    %end
%end
