# Catalog queries for columns
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

#NOTE: For columns and other table object is needed to pass the table name as
#      well the schema name of the parent table in the both data retrieving methods (list/attribs)

%if @{list} %then
[ SELECT cl.attnum AS oid, cl.attname AS name FROM pg_attribute AS cl
   LEFT JOIN pg_class AS tb ON tb.oid = cl.attrelid
   LEFT JOIN pg_namespace AS ns ON ns.oid = tb.relnamespace
   WHERE cl.attisdropped IS FALSE AND relname=]'@{table}' [ AND nspname= ] '@{schema}'
#    [ AND attnum >= 0  AND attinhcount = 0 ORDER BY attnum ASC ]
   [ AND attnum >= 0  ORDER BY attnum ASC ]
%else
    %if @{attribs} %then
     [SELECT cl.attnum AS oid, cl.attname AS name, cl.attnotnull AS not_null_bool,
	     cl.attacl AS permission, df.adsrc AS default_value,
	     ds.description AS comment, ]

     # This subquery retrieve the schema name for the type when it is undeer public schema.
     # This is necessary because pgModeler identifies user-defined types by the complete
     # type signature [schema].[typename]
     [(SELECT
	CASE
	 WHEN ns.nspname='public' THEN ns.nspname || '.'
	 ELSE ''
	END
	FROM pg_namespace AS ns
	LEFT JOIN pg_type AS tp ON tp.typnamespace=ns.oid
	WHERE tp.oid=cl.atttypid) || format_type(atttypid,atttypmod) AS type, ]

      %if @{pgsql90} %then
       [ NULL AS collation ]
      %else
       [ cl.attcollation AS collation ]
      %end

     [ FROM pg_attribute AS cl
       LEFT JOIN pg_attrdef AS df ON df.adrelid=cl.attrelid AND df.adnum=cl.attnum
       LEFT JOIN pg_description AS ds ON ds.objoid=cl.attrelid AND ds.objsubid=cl.attnum
       LEFT JOIN pg_class AS tb ON tb.oid = cl.attrelid
       LEFT JOIN pg_namespace AS ns ON ns.oid = tb.relnamespace
       WHERE  cl.attisdropped IS FALSE AND relname= ] '@{table}'
       [ AND nspname= ] '@{schema}'
       #[ AND attnum >= 0  AND attinhcount = 0 ]
       [ AND attnum >= 0  ]

       %if @{filter-oids} %then
	[ AND cl.attnum IN (] @{filter-oids} )
       %end

       [ ORDER BY attnum ASC ]
    %end
%end
