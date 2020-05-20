# Catalog queries for operator family
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [SELECT op.oid, op.opfname || ' ] $ob [' || am.amname || '] $cb [' AS name, 
   ns.nspname AS parent, 'schema' AS parent_type
   FROM pg_opfamily AS op 
   LEFT JOIN pg_namespace AS ns ON op.opfnamespace = ns.oid
   LEFT JOIN pg_am AS am ON op.opfmethod = am.oid ]
  
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
	[ SELECT op.oid, op.opfname AS name, am.amname AS index_type,
		 op.opfowner AS owner, op.opfnamespace AS schema, ]

	({comment}) [ AS comment ]

	[ FROM pg_opfamily AS op
	  LEFT JOIN pg_am AS am ON op.opfmethod = am.oid ]

	%if {schema} %then
	  [ LEFT JOIN pg_namespace AS ns ON op.opfnamespace = ns.oid ]
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
