# Catalog queries for casts
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
   [SELECT cs.oid, 'cast('|| castsource::regtype::text || ',' || casttarget::regtype::text || ')' AS name,
     current_database() AS parent,
     'database' AS parent_type
     FROM pg_cast AS cs]

  %if {last-sys-oid} %then
   [ WHERE oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if {not-ext-object} %then
    %if {last-sys-oid} %then
      [ AND ]
    %else
     [ WHERE ]
    %end
    ( {not-ext-object} )
  %end

%else
    %if {attribs} %then
      [SELECT oid, 'cast('|| castsource::regtype::text || ',' || casttarget::regtype::text || ')' AS name,
	      castsource AS source_type, casttarget AS destiny_type, castfunc AS function,

	 CASE castcontext
	    WHEN  'a' THEN 'assignment'
	    WHEN  'i' THEN 'implicit'
	    ELSE 'explicit'
	 END AS cast_type,

	 CASE castmethod
	    WHEN  'b' THEN TRUE
	    ELSE FALSE
	 END AS io_cast_bool, ]

      ({comment}) [ AS comment ]

     [ FROM pg_cast AS cs]

      %if {last-sys-oid} %then
       [ WHERE oid ] {oid-filter-op} $sp {last-sys-oid}
      %end

      %if {filter-oids} %then

        %if {last-sys-oid} %then
         [ AND]
        %else
         [ WHERE ]
        %end

        [ oid IN (] {filter-oids} )
      %end

      %if {not-ext-object} %then
         %if {last-sys-oid} %or {filter-oids} %then
	   [ AND ]
	 %else
	   [ WHERE ]
	 %end
	( {not-ext-object} )
      %end
    %end
%end
