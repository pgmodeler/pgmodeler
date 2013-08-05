# Catalog queries for domains
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT dm.oid, dm.typname AS name FROM pg_type AS dm
    INNER JOIN information_schema.domains AS _dm1 ON dm.typname=_dm1.domain_name ]

  %if @{schema} %then
    [ WHERE domain_schema=] '@{schema}'
  %end
%else
    %if @{attribs} %then
     [SELECT dm.oid, dm.typname AS name, dm.typowner AS owner, dm.typacl AS permissions,
	     dm.typcollation, dm.typnotnull AS not_null_bool, _dm1.data_type AS type, _dm1.character_maximum_length AS length,

	   CASE
	    WHEN _dm1.numeric_precision IS NOT NULL THEN _dm1.numeric_precision
	    WHEN _dm1.datetime_precision IS NOT NULL THEN _dm1.datetime_precision
	    WHEN _dm1.interval_precision IS NOT NULL THEN _dm1.interval_precision
	    ELSE NULL
	   END AS precision,

	   _dm1.interval_type, _dm1.domain_default AS default_value,
	   cn.conname AS constraint, cn.consrc AS expression, ]

      (@{comment}) [ AS comment, ]
      (@{from-extension}) [ AS from_extension_bool ]

      [ FROM pg_type AS dm
	INNER JOIN pg_constraint AS cn ON cn.contypid=dm.oid
	INNER JOIN information_schema.domains AS _dm1 ON dm.typname=_dm1.domain_name ]

      %if @{filter-oids} %or @{schema} %then
      [ WHERE ]
	%if @{filter-oids} %then
	  [ dm.oid IN (] @{filter-oids} )

	   %if @{schema} %then
	     [ AND ]
	   %end
	 %end

	 %if @{schema} %then
	  [ _dm1.domain_schema= ] '@{schema}'
	 %end
       %end
    %end
%end
