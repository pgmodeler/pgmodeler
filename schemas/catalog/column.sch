# Catalog queries for columns
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

#NOTE: For columns and other table object is needed to pass the table name as
#      well the schema name of the parent table in the both data retrieving methods (list/attribs)

%if @{list} %then
[ SELECT cl.attnum AS oid, cl.attname AS name FROM pg_attribute AS cl
   LEFT JOIN pg_class AS tb ON tb.oid = cl.attrelid
   LEFT JOIN pg_namespace AS ns ON ns.oid = tb.relnamespace
   WHERE relname=]'@{table}' [ AND nspname= ] '@{schema}' [ AND attnum >= 0 ORDER BY attnum ASC ]
%else
    %if @{attribs} %then
     [SELECT cl.attname AS name, cl.atttypid AS type, cl.attlen AS length, cl.attndims AS dimension,
	     cl.attnotnull AS not_null_bool, cl.attacl AS permissions, df.adsrc AS default_value,
	     ds.description AS comment, ]

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
       WHERE relname= ] '@{table}' [ AND nspname= ] '@{schema}' [ AND attnum >= 0 ]

       %if @{filter-oids} %then
	[ AND cl.attnum IN (] @{filter-oids} )
       %end

       [ ORDER BY attnum ASC ]
    %end
%end
