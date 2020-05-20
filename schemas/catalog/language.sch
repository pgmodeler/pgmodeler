# Catalog queries for languages
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [SELECT oid, lanname AS name, current_database() AS parent, 'database' AS parent_type
   FROM pg_language ]

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
  
  %if {name-filter} %then
    %if {last-sys-oid} %or {not-ext-object} %then
      [ AND ]
    %else
      [ WHERE ]
    %end
  
    ( {name-filter} )
  %end

%else
    %if {attribs} %then
      [SELECT oid, lanname AS name, lanpltrusted AS trusted_bool,
	      lanplcallfoid AS handler, laninline AS inline,
	      lanvalidator AS validator, lanacl AS permission, lanowner AS owner, ]

      ({comment}) [ AS comment ]

      [ FROM pg_language ]

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

       %if {filter-oids} %then
	 %if {last-sys-oid} %or {not-ext-object} %then
	   [ AND ]
	 %else
	   [ WHERE ]
	 %end
	 [ oid IN (] {filter-oids} )
       %end
    %end
%end
