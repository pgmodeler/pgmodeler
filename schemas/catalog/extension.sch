# Catalog queries for extensions
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

# Extension exists only on PostgreSQL >= 9.1
%if {list} %and ({pgsql-ver} != "9.0") %then
   [SELECT ex.oid, extname AS name FROM pg_extension AS ex ]

   %if {schema} %then
    [ LEFT JOIN pg_namespace AS ns ON ex.extnamespace = ns.oid
      WHERE ns.nspname = ] '{schema}'
   %end

  %if {last-sys-oid} %then
    %if {schema} %then
     [ AND ]
    %else
     [ WHERE ]
    %end
     [ ex.oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

%else
    %if {attribs} %and ({pgsql-ver} != "9.0")  %then
	[SELECT ex.oid, ex.extname AS name, ex.extversion AS cur_version, NULL AS old_version,
		ex.extowner AS owner, ex.extnamespace AS schema,
	  (SELECT CASE
		    WHEN  count(objid) >= 1 THEN TRUE
		    ELSE FALSE
		   END
	    FROM pg_depend AS _dp
	    LEFT JOIN pg_extension AS _ex ON _ex.oid=_dp.objid
	    WHERE  _dp.refobjid = ex.oid AND _dp.objid::regtype::text SIMILAR TO '(' || ex.extnamespace::regnamespace::text || '.)?(' || ex.extname  || ')'
	    AND _dp.classid::regclass::text = 'pg_type') AS handles_type_bool, ]

	  ({comment}) [ AS comment ]

	 [ FROM pg_extension AS ex ]

      %if {schema} %then
       [ LEFT JOIN pg_namespace AS ns ON ex.extnamespace = ns.oid ]
      %end

       %if {filter-oids} %or {schema} %then
	 [ WHERE ]

	 %if {filter-oids} %then
	   [ ex.oid IN (] {filter-oids} )

	   %if {schema} %then
	     [ AND ]
	   %end
	 %end

	 %if {schema} %then
	   [ nspname = ] '{schema}'
	 %end
       %end

       %if {last-sys-oid} %then
	 %if {filter-oids} %or {schema} %then
	   [ AND ]
	 %else
	   [ WHERE ]
	 %end
	 [ ex.oid ] {oid-filter-op} $sp {last-sys-oid}
       %end

    %end
%end
