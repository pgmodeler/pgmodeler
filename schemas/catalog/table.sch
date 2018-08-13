# Catalog queries for tables
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [ SELECT tb.oid, tb.relname AS name FROM pg_class AS tb]

  %if {schema} %then
    [ LEFT JOIN pg_namespace AS ns ON ns.oid=tb.relnamespace
      WHERE tb.relkind='r' AND ns.nspname= ] '{schema}'
  %else
    [ WHERE tb.relkind='r']
  %end

  %if {last-sys-oid} %then
     [ AND tb.oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if {not-ext-object} %then
    [ AND ] ( {not-ext-object} )
  %end

%else
    %if {attribs} %then
    [SELECT tb.oid, tb.relname AS name, tb.relnamespace AS schema, tb.relowner AS owner,
            tb.reltablespace AS tablespace, tb.relacl AS permission, relhasoids AS oids_bool, ]

    %if ({pgsql-ver} == "9.0") %then
     [ FALSE AS unlogged_bool, ]
    %else
     [ CASE relpersistence
         WHEN 'u' THEN TRUE
         ELSE FALSE
       END
       AS unlogged_bool, ]
    %end
    
    %if ({pgsql-ver} <f "9.5") %then
        [ FALSE AS rls_enabled_bool, ]
        [ FALSE AS rls_forced_bool, ]
    %else
        [ tb.relrowsecurity AS rls_enabled_bool, ]
        [ tb.relforcerowsecurity AS rls_forced_bool, ]
    %end

    [(SELECT array_agg(inhparent) AS parents FROM pg_inherits WHERE inhrelid = tb.oid)],

    ({comment}) [ AS comment ]
    
    [ , st.seq_scan AS seq_scan, st.seq_tup_read AS seq_scan_read, st.idx_scan AS index_scan, st.idx_tup_fetch AS index_scan_read, 
      st.n_tup_ins AS tuples_ins, st.n_tup_upd AS tuples_upd, st.n_tup_del AS tuples_del, st.n_live_tup AS row_amount, 
      st.last_vacuum, st.last_autovacuum, st.last_analyze, st.vacuum_count, st.autovacuum_count, st.analyze_count, st.autoanalyze_count ]
  
    [ FROM pg_class AS tb
      LEFT JOIN pg_tables AS _tb1 ON _tb1.tablename=tb.relname 
      LEFT JOIN pg_stat_all_tables AS st ON st.relid=tb.oid
      WHERE tb.relkind='r' ]

    %if {last-sys-oid} %then
        [ AND tb.oid ] {oid-filter-op} $sp {last-sys-oid}
    %end

    %if {not-ext-object} %then
        [ AND ] (  {not-ext-object} )
    %end

      %if {filter-oids} %or {schema} %then
        [ AND ]
        %if {filter-oids} %then
            [ tb.oid IN (] {filter-oids} )

            %if {schema} %then
                [ AND ]
            %end
        %end

        %if {schema} %then
            [ _tb1.schemaname= ] '{schema}'
        %end
     %end

    %end
    
%end
