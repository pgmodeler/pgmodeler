# Catalog queries for user defined types
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [SELECT tp.oid, replace(replace(tp.oid::regtype::text,'"', ''), ns.nspname || '.', '') AS name, 
    ns.nspname AS parent, 'schema' AS parent_type 
    FROM pg_type AS tp 
    LEFT JOIN pg_namespace AS ns ON tp.typnamespace = ns.oid ]

  %if {schema} %then
    [ WHERE ns.nspname = ] '{schema}' [ AND ]
  %else
    [ WHERE ]
  %end

  [ typtype IN ('p','b','c','e','r')  AND typname NOT LIKE 'pg_%' ]

  #Excluding types related to tables/views/sequeces/materialized views/foreign tables
  %if {filter-tab-types} %then
   [  AND (SELECT count(oid) FROM pg_class WHERE relname=typname AND relkind IN ('r','S','v','m','p','f'))=0 ]
  %end

  %if {exc-builtin-arrays} %then
    #The tp.typtype='p' indicates to include pseudo-type even if they are an array construction
    [ AND (tp.typtype='p' OR tp.typarray > 0) ]
  %end

  %if {last-sys-oid} %then
    [ AND tp.oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if {not-ext-object} %then
    [ AND ] (  {not-ext-object} )
  %end
  
  %if {name-filter} %then
    [ AND ] ( {name-filter} )
  %end

%else
    %if {attribs} %then
    [SELECT tp.oid, replace(replace(tp.oid::regtype::text,'"', ''), ns.nspname || '.', '') AS name, tp.typnamespace AS schema, tp.typowner AS owner, ]
    
    #Retrieve the OID for table/view/sequence that generates the composite type
    [ 
        CASE 
        WHEN cl.relkind = 'r' THEN 'table'
        WHEN cl.relkind = 'S' THEN 'sequence'
        WHEN cl.relkind = 'v' THEN 'view'
        WHEN cl.relkind = 'm' THEN 'view'
        WHEN cl.relkind = 'p' THEN 'table'
        WHEN cl.relkind = 'f' THEN 'foreigntable'
        END AS type_class, tp.typrelid AS object_id, ]

    #TODO: Discover which field is the acl for user defined types on PgSQL 9.0
		%if ({pgsql-ver} <=f "9.1") %then
      [ NULL AS permission, ]
    %else
      [ tp.typacl AS permission,]
    %end

    %if ({pgsql-ver} == "9.0") %then
     [ NULL AS collations, ]
    %else
     [ tp.typcollation AS collation, ]
    %end

    [   CASE
          WHEN typtype = 'e' THEN 'enumeration'
          WHEN typtype = 'b' THEN 'base'
          WHEN typtype = 'c' THEN 'composite'
          WHEN typtype = 'r' THEN 'range'
          WHEN typtype = 'd' THEN 'domain'
          WHEN typtype = 'p' THEN 'pseudo'
        END AS configuration, ] 

    # Retrieve the enumaration labels (is null when the type is not an enumeration)
    [   CASE WHEN typtype = 'e' THEN (SELECT array_agg(enumlabel) FROM pg_enum WHERE enumtypid=tp.oid)
        END AS enumerations, ]

    %if ({pgsql-ver} >=f "9.3") %then
       [ ta.typeattrib, ]            
    %else
        # Retrieve the composite attributes in the sequence: name, type, dimension (for array types), collation (pgsql > 9.0)
        # separating them by colon.
        # (this field is null when the type is not a composite)     
        [ CASE WHEN typtype = 'c' THEN 
        (
            SELECT array_agg(attname ||':'|| 
            (SELECT CASE WHEN ns.nspname='public' THEN ns.nspname || '.' ELSE '' END 
            FROM pg_namespace AS ns
            LEFT JOIN pg_type AS _tp ON _tp.oid = at.atttypid WHERE ns.oid=_tp.typnamespace) || 
            format_type(atttypid,atttypmod) || ':' || ] %if ({pgsql-ver} == "9.0") %then '0' %else attcollation %end [) FROM pg_attribute AS at 
            WHERE attrelid = cl.oid
        )
        END AS typeattrib, ]
    %end
       
    # Retrieve the range type attributes (is null when the type is not a range) (pgsql >= 9.2)
		%if ({pgsql-ver} >=f "9.2") %then
    [ CASE WHEN typtype = 'r' THEN (SELECT string_to_array(rngsubtype||':'||rngcollation||':'||rngsubopc::oid||':'||
					   rngcanonical::oid||':'||rngsubdiff::oid, ':')
                                    FROM pg_range WHERE rngtypid=tp.oid)
      END AS range_attribs, ]
    %end

    [ tp.typinput::oid AS input, tp.typoutput::oid AS output, tp.typreceive::oid AS receive, tp.typsend::oid AS send,
      tp.typmodin::oid AS tpmodin, tp.typmodout::oid AS tpmodout, tp.typanalyze::oid AS analyze,
      tp.typlen AS internal_length, tp.typbyval AS by_value_bool, 

      CASE tp.typalign 
        WHEN 'i' THEN 'integer'
        WHEN 'c' THEN 'char'
        WHEN 's' THEN 'smallint'
        ELSE 'double precision'
      END AS alignment,

      CASE tp.typstorage 
        WHEN 'p' THEN 'plain'
        WHEN 'm' THEN 'main'
        WHEN 'x' THEN 'extended'
        ELSE 'external'
      END AS storage,

     tp.typdefault AS default_value, tp.typelem AS element, 
     tp.typdelim AS delimiter, tp.typispreferred AS preferred_bool,
     tp.typcategory AS category, ]

     %if ({pgsql-ver} == "9.0") %then
      [ FALSE AS collatable_bool, ]
     %else
      [ CASE WHEN tp.typcollation <> 0 THEN TRUE
	 ELSE FALSE
	END AS collatable_bool, ]
     %end
     
    ({comment}) [ AS comment ]

    [ FROM pg_type AS tp
      LEFT JOIN pg_class AS cl ON cl.oid = tp.typrelid
    LEFT JOIN pg_namespace AS ns ON tp.typnamespace = ns.oid ]
      
    # Retrieving composite type attributes using lateral join on 9.3+ (way more performatic)
    %if ({pgsql-ver} >=f "9.3") %then                
    [ LEFT JOIN LATERAl (
      SELECT array_agg(
           attname
           || ':'
           || CASE WHEN ns.nspname='public' THEN ns.nspname || '.' ELSE '' END
           || format_type(atttypid,atttypmod) 
           || ':'
           || attcollation
      ) AS typeattrib
      FROM pg_attribute AS at
      JOIN pg_type AS _tp on _tp.oid = at.atttypid
      JOIN pg_namespace AS ns ON ns.oid = _tp.typnamespace
      WHERE at.attrelid = cl.oid
     ) ta ON (TRUE) ]
    %end

    #Excluding types related to tables/views/sequeces
    [ WHERE typtype IN ('p','b','c','e','r', 'd') AND typname NOT LIKE 'pg_%' ]

    #Excluding types related to tables/views/sequeces/materialized views
    %if {filter-tab-types} %then
     [  AND (SELECT count(oid) FROM pg_class WHERE relname=typname AND relkind IN ('r','S','v','m','p','f'))=0 ]
    %end

    %if {exc-builtin-arrays}  %then
      #The tp.typtype='p' indicates to include pseudo-type even if they are an array construction
      [ AND (tp.typtype='p' OR tp.typarray > 0) ]
    %end

    %if {last-sys-oid} %then
      [ AND tp.oid ] {oid-filter-op} $sp {last-sys-oid}
    %end

    %if {not-ext-object} %then
      [ AND ] (  {not-ext-object} )
    %end
    
       %if {filter-oids} %or {schema} %then
          [ AND ]
          %if {filter-oids} %then
            [ tp.oid IN (] {filter-oids} )

            %if {schema} %then
              [ AND ]
            %end
          %end

          %if {schema} %then
            [ ns.nspname = ] '{schema}'
          %end
       %end
       
    %end
%end
