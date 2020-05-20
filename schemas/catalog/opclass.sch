# Catalog queries for operator class
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [SELECT op.oid, opcname || ' ] $ob [' || am.amname || '] $cb [' AS name, 
   ns.nspname AS parent, 'schema' AS parent_type
   FROM pg_opclass AS op 
   LEFT JOIN pg_namespace AS ns ON op.opcnamespace = ns.oid 
   LEFT JOIN pg_am AS am ON op.opcmethod = am.oid ]

  %if {schema} %then
    [ WHERE ns.nspname = ] '{schema}'
  %end

  %if {last-sys-oid} %then
    %if {schema} %then
     [ AND ]
    %else
     [ WHERE ]
    %end
     [ op.oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if {not-ext-object} %then
    %if {last-sys-oid} %or {schema} %then
      [ AND ]
    %else
      [ WHERE ]
    %end
    (  {not-ext-object} )
  %end
  
  %if {name-filter} %then
  
    %if {last-sys-oid} %or {schema} %or {not-ext-object} %then
      [ AND ]
    %else
      [ WHERE ]
    %end
  
    ( {name-filter} )
  %end

%else
    %if {attribs} %then
      [SELECT op.oid, op.opcname AS name, op.opcnamespace AS schema, op.opcowner AS owner,
	      op.opcfamily AS family, op.opcintype AS type, op.opcdefault AS default_bool,
	      op.opckeytype AS storage, am.amname AS index_type, ]

      [(SELECT array_agg(amopstrategy::text || ':' || amopopr::text || ':' || ] %if ({pgsql-ver} == "9.0") %then 0 %else amopsortfamily::text %end [)]
      [ FROM pg_amop AS ap WHERE ap.amopfamily=op.opcfamily AND ap.amopmethod=am.oid) AS operator, ]

      [(SELECT array_agg(amprocnum::text || ':' || amproc::oid::text)
	 FROM pg_amproc AS _ap1 WHERE _ap1.amprocfamily=op.opcfamily) AS function, ]

      ({comment}) [ AS comment ]


      [ FROM pg_opclass AS op
	LEFT JOIN pg_am AS am ON op.opcmethod = am.oid ]

      %if {schema} %then
	[ LEFT JOIN pg_namespace AS ns ON op.opcnamespace = ns.oid ]
      %end

      %if {filter-oids} %or {schema} %then
	[ WHERE ]
	  %if {filter-oids} %then
	   [ op.oid IN (] {filter-oids} )

	    %if {schema} %then
	      [ AND ]
	    %end
	  %end

	  %if {schema} %then
	   [ ns.nspname = ] '{schema}'
	  %end
      %end

      %if {last-sys-oid} %then
	%if {schema} %or {filter-oids} %then
	  [ AND ]
	%else
	  [ WHERE ]
	%end
	[ op.oid ] {oid-filter-op} $sp {last-sys-oid}
      %end

      %if {not-ext-object} %then
	%if {last-sys-oid} %or {schema} %or {filter-oids} %then
	  [ AND ]
	%else
	  [ WHERE ]
	%end
	(  {not-ext-object} )
      %end

    %end
%end
