# Catalog queries for rules
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
[SELECT rl.oid, rl.rulename AS name
  FROM (
	SELECT rw.oid, rw.*
	FROM pg_rewrite AS rw
	WHERE rw.ev_type <> '1'::"char"
  ) AS rl
  LEFT JOIN pg_class cl ON cl.oid = rl.ev_class AND cl.relkind IN ('r','v') ]

  %if {schema} %then
    [ LEFT JOIN pg_namespace AS ns ON ns.oid = cl.relnamespace
      WHERE ns.nspname= ] '{schema}'

    %if {table} %then
     [ AND cl.relname=]'{table}'
    %end
  %end

  %if {last-sys-oid} %then
    %if {schema} %then
      [ AND ]
    %else
      [ WHERE ]
    %end

    [ rl.oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if {not-ext-object} %then
    %if %not {last-sys-oid} %and %not {schema} %then
      [ WHERE ]
    %else
      [ AND ]
    %end
      ( {not-ext-object} )
  %end

%else
    %if {attribs} %then
      [SELECT rl.oid, rl.rulename AS name, cl.oid as table,
	      pg_get_ruledef(rl.oid) AS commands,
	      ds.description AS comment,
	CASE
	 WHEN rl.is_instead IS TRUE THEN 'INSTEAD'
	 ELSE 'ALSO'
	END exec_type,

	CASE rl.ev_type
	 WHEN '1'::"char" THEN 'ON SELECT'::text
	 WHEN '2'::"char" THEN 'ON UPDATE'::text
	 WHEN '3'::"char" THEN 'ON INSERT'::text
	 WHEN '4'::"char" THEN 'ON DELETE'::text
	 ELSE NULL::text
	END AS event_type

       FROM (
	SELECT rw.oid, rw.*
	FROM pg_rewrite AS rw
       ) AS rl

      LEFT JOIN pg_class AS cl ON cl.oid = rl.ev_class AND cl.relkind IN ('r','v')
      LEFT JOIN pg_description ds ON ds.objoid = rl.oid ]

      %if {schema} %then
	[ LEFT JOIN pg_namespace AS ns ON ns.oid = cl.relnamespace
	  WHERE ns.nspname= ] '{schema}'

	%if {table} %then
	  [ AND cl.relname=]'{table}'
	%end
      %end

      %if {last-sys-oid} %then
	%if {schema} %then
	 [ AND ]
	%else
	 [ WHERE ]
	%end

	[ rl.oid ] {oid-filter-op} $sp {last-sys-oid}
      %end

      %if {filter-oids} %then
        %if %not {last-sys-oid} %then
          [ WHERE ]
        %else
          [ AND ]
        %end
        [ rl.oid IN (] {filter-oids} )
      %end

        %if {not-ext-object} %then
          %if %not {last-sys-oid} %and %not {filter-oids} %and %not {schema} %then
            [ WHERE ]
          %else
            [ AND ]
          %end
          ( {not-ext-object} )
        %end

    %end
%end
