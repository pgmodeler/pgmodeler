# Catalog queries for domains
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT dm.oid, dm.typname AS name FROM pg_type AS dm
    INNER JOIN information_schema.domains AS _dm1 ON dm.typname=_dm1.domain_name ]

  %if @{schema} %then
    [ WHERE domain_schema=] '@{schema}'
  %end

  %if @{last-sys-oid} %then
    %if @{schema} %then
     [ AND ]
    %else
     [ WHERE ]
    %end
     [ dm.oid ] @{oid-filter-op} $sp @{last-sys-oid}
  %end

  %if @{not-ext-object} %then
    %if @{last-sys-oid} %or @{schema} %then
      [ AND ]
    %else
      [ WHERE ]
    %end
    (  @{not-ext-object} )
  %end

%else
    %if @{attribs} %then
     [SELECT dm.oid, dm.typname AS name, dm.typowner AS owner, dm.typnamespace AS schema, dm.typndims AS dimension,
	     dm.typbasetype AS type, ]

	#TODO: Discover which field is the acl for domain on PgSQL 9.0 and 9.1
	%if %not @{pgsql92} %and %not @{pgsql93} %then
	 [ NULL AS permission, NULL AS collation, ]
	%else
	 [ dm.typacl AS permission, dm.typcollation AS collation, ]
	%end

	[ CASE
	    WHEN _dm1.numeric_precision_radix IS NOT NULL THEN _dm1.numeric_precision_radix
	    ELSE _dm1.character_maximum_length
	  END AS length,

	  CASE
	    WHEN _dm1.numeric_precision_radix IS NOT NULL THEN _dm1.numeric_scale ] %if %not @{pgsql92} %and %not @{pgsql93} %then [::varchar] %end
	[   WHEN _dm1.datetime_precision IS NOT NULL THEN _dm1.datetime_precision ] %if %not @{pgsql92} %and %not @{pgsql93} %then [::varchar] %end
	[   WHEN _dm1.interval_precision IS NOT NULL THEN _dm1.interval_precision ] %if %not @{pgsql92} %and %not @{pgsql93} %then [::varchar] %end
	[   ELSE NULL
	 END AS precision,

	  dm.typnotnull AS not_null_bool,
	  _dm1.interval_type, _dm1.domain_default AS default_value,
	  cn.conname AS constraint, cn.consrc AS expression, ]

      (@{comment}) [ AS comment ]

      [ FROM pg_type AS dm
	LEFT JOIN pg_constraint AS cn ON cn.contypid=dm.oid
	LEFT JOIN information_schema.domains AS _dm1 ON dm.typname=_dm1.domain_name ]

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

      %if @{last-sys-oid} %then
	%if @{filter-oids} %or @{schema} %then
	  [ AND ]
	%else
	  [ WHERE ]
	%end
	[ dm.oid ] @{oid-filter-op} $sp @{last-sys-oid}
      %end

      %if @{not-ext-object} %then
	%if @{last-sys-oid} %or @{filter-oids} %or @{schema} %then
	  [ AND ]
	%else
	  [ WHERE ]
	%end
	( @{not-ext-object} )
      %end
    %end
%end
