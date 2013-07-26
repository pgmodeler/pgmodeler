# Catalog queries for user defined types
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT tp.oid, tp.typname AS name FROM pg_type AS tp ]

  %if @{schema} %then
    [ LEFT JOIN pg_namespace AS ns ON tp.typnamespace = ns.oid
       WHERE ns.nspname = ] '@{schema}' [ AND ]
  %else
    [ WHERE ]
  %end

  [ typtype IN ('b','c','e','r')]

%else
    %if @{attribs} %then

    [SELECT tp.oid, tp.typname AS name, tp.typnamespace AS schema, tp.typowner AS owner,
            tp.typacl AS permissions, tp.typcollation AS collation,
        CASE 
          WHEN typtype = 'e' THEN 'enumeration'
          WHEN typtype = 'b' THEN 'base'    
          WHEN typtype = 'c' THEN 'composite'
          WHEN typtype = 'r' THEN 'range'
        END AS configuration, ] 

    # Retrieve the enumaration labels (is null when the type is not an enumeration)
    [   CASE WHEN typtype = 'e' THEN (SELECT array_agg(enumlabel) FROM pg_enum WHERE enumtypid=tp.oid)
        END AS enumerations, ]

    # Retrieve the composite attributes in the sequence: name, type, dimension (for array types), collation 
    # separating them by colon.
    # (this field is null when the type is not a composite)
    [   CASE WHEN typtype = 'c' THEN (SELECT array_agg(attname ||':'|| atttypid ||':'|| attndims||':'|| attcollation) 
                                FROM pg_attribute 
                                WHERE attrelid=(SELECT oid FROM pg_class WHERE reltype=tp.oid))
         END AS typeattrib, ]
    # Retrieve the range type attributes (is null when the type is not a range)
    [ CASE WHEN typtype = 'r' THEN (SELECT string_to_array(rngsubtype||','||rngcollation||','||rngsubopc::oid||','||
                                           rngcanonical::oid||','||rngsubdiff::oid, ',') 
                                    FROM pg_range WHERE rngtypid=tp.oid)
      END AS rangeattribs, ]

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
     tp.typcategory AS category, 

     CASE WHEN tp.typcollation <> 0 THEN TRUE
      ELSE FALSE
     END AS collatable_bool, ]
     
    (@{comment}) [ AS comment, ]
    (@{from-extension}) [ AS from_extension_bool ]

    [ FROM pg_type AS tp ]

    %if @{schema} %then
      [ LEFT JOIN pg_namespace AS ns ON tp.typnamespace = ns.oid ]
    %end

    [ WHERE typtype IN ('b','c','e','r') ]
    
    %if @{filter-oids} %or @{schema} %then
	[ AND ]
	  %if @{filter-oids} %then
	   [ tp.oid IN (] @{filter-oids} )

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
