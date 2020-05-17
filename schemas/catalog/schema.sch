# Catalog queries for schemas
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [SELECT oid, nspname AS name, current_database() AS parent, 'database' AS parent_type FROM pg_namespace
    WHERE (nspname NOT LIKE 'pg_temp%' AND nspname NOT LIKE 'pg_toast%') ]

  %if {last-sys-oid} %then
    [ AND ( oid ] {oid-filter-op} $sp {last-sys-oid} [ OR nspname = 'public' ) ]
  %end

  %if {not-ext-object} %then
    [ AND ] ( {not-ext-object} )
  %end 
  
  %if {name-filter} %then
    [ AND ] ( {name-filter} )
  %end
  
%else
    %if {attribs} %then
      [SELECT oid, nspname AS name, nspacl AS permission, nspowner AS owner, ]

	({comment}) [ AS comment ]

       [ FROM pg_namespace
	  WHERE (nspname NOT LIKE 'pg_temp%' AND nspname NOT LIKE 'pg_toast%') ]

       %if {last-sys-oid} %then
         [ AND (oid ] {oid-filter-op} $sp {last-sys-oid} [ OR nspname = 'public' ) ]
       %end

       %if {filter-oids} %then
	 [ AND oid IN (] {filter-oids} )
       %end

      %if {not-ext-object} %then
	 [ AND ] ( {not-ext-object} )
      %end

    %end
%end
