# Catalog queries for tablespaces
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [SELECT oid, spcname AS name, current_database() AS parent, 'database' AS parent_type
   FROM pg_tablespace ]

  %if {last-sys-oid} %then
   [ WHERE oid ] {oid-filter-op} $sp {last-sys-oid}
  %end
  
  %if {name-filter} %then        
    %if {last-sys-oid} %then
        [ AND ] 
    %else
        [ WHERE ]
    %end        
        
    ( {name-filter} )    
  %end

%else
    %if {attribs} %then
	[SELECT oid, spcname AS name, spcacl AS permission, spcowner AS owner, ]

				%if ({pgsql-ver} >=f "9.2") %then
	  [ pg_tablespace_location(oid) AS directory, ]
	%else
	  [ spclocation AS directory, ]
	%end

	({comment}) [ AS comment ]

	[ FROM pg_tablespace ]

	%if {last-sys-oid} %then
	  [ WHERE oid ] {oid-filter-op} $sp {last-sys-oid}
	%end

	%if {filter-oids} %then
	  %if {last-sys-oid} %then
	   [ AND ]
	  %else
	   [ WHERE ]
	  %end

	 [ oid IN (] {filter-oids} )
	%end
    %end
%end
