# Catalog queries for tables
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [ SELECT tb.oid, tb.relname AS name, ns.nspname AS parent, 'schema' AS parent_type
    FROM pg_class AS tb 
    LEFT JOIN pg_namespace AS ns ON ns.oid=tb.relnamespace ]

  %if {schema} %then
    [ WHERE tb.relkind IN ('r','p') AND ns.nspname= ] '{schema}'
  %else
    [ WHERE tb.relkind IN ('r','p')]
  %end

  %if {last-sys-oid} %then
     [ AND tb.oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if {not-ext-object} %then
    [ AND ] ( {not-ext-object} )
  %end
  
  %if {name-filter} %then
    [ AND ] ( {name-filter} )
  %end

%else
    %if {attribs} %then
    [SELECT tb.oid, tb.relname AS name, tb.relnamespace AS schema, tb.relowner AS owner,
            tb.reltablespace AS tablespace, tb.relacl AS permission, ]             
    
    %if ({pgsql-ver} <f "12.0") %then
        [ relhasoids AS oids_bool, ]
    %else
        [ FALSE AS oids_bool, ]
    %end

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

    [(SELECT array_agg(inhparent) AS parents FROM pg_inherits WHERE inhrelid = tb.oid]
    
    # In PostgreSQL 10+ we need to separate partitioned tables from parent tables
    %if ({pgsql-ver} >=f "10.0") %then
        [ AND inhparent NOT IN (SELECT partrelid FROM pg_partitioned_table)]
    %end
    
    [)],
    
    %if ({pgsql-ver} >=f "10.0") %then
        [ CASE relkind 
            WHEN 'p' THEN TRUE
            ELSE FALSE 
          END AS is_partitioned_bool, 
        
          CASE relispartition
            WHEN TRUE THEN
               (SELECT inhparent FROM pg_inherits WHERE inhrelid = tb.oid)
            ELSE
                NULL 
          END AS partitioned_table, 
          
          pg_get_expr(relpartbound, tb.oid) AS partition_bound_expr,
            
        ]
    %else
        [ FALSE AS is_partitioned_bool, NULL AS partitioned_table, NULL AS partition_bound_expr,] 
    %end
     

    ({comment}) [ AS comment ]
    
    [ , st.n_tup_ins AS tuples_ins, st.n_tup_upd AS tuples_upd, st.n_tup_del AS tuples_del, st.n_live_tup AS row_amount,
        st.n_dead_tup AS dead_rows_amount, st.last_vacuum, st.last_autovacuum, st.last_analyze, ]
        
    %if ({pgsql-ver} >=f "10.0") %then    
    [ CASE partstrat
        WHEN 'l' then 'LIST'
        WHEN 'r' then 'RANGE'
        ELSE 'HASH'
      END AS partitioning,
      pg_get_expr(partexprs, partrelid) AS part_key_exprs,
      partattrs::oid] $ob $cb [ AS part_key_cols, 
      partclass::oid] $ob $cb [ AS part_key_opcls, 
      partcollation::oid] $ob $cb [ AS part_key_colls ]
    %else
      [ NULL AS partitioning, NULL AS part_key_exprs, NULL AS part_key_cols,
        NULL AS part_key_opcls, NULL AS part_key_colls ]
    %end
  
    [ FROM pg_class AS tb
      LEFT JOIN pg_tables AS _tb1 ON _tb1.tablename=tb.relname 
      LEFT JOIN pg_stat_all_tables AS st ON st.relid=tb.oid ]
    
    %if ({pgsql-ver} >=f "10.0") %then
      [ LEFT JOIN pg_partitioned_table AS pt ON pt.partrelid = tb.oid ]
    %end

      
    [ WHERE tb.relkind IN ('r','p') ]

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
