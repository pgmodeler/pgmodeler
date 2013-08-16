# Catalog queries for schemas
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT oid, nspname AS name FROM pg_namespace ]

  %if @{last-sys-oid} %then
   [ WHERE oid ] @{oid-filter-op} $sp @{last-sys-oid} [ OR nspname = 'public' ]
  %end

  %if @{from-extension} %then
     %if @{last-sys-oid} %then
       [ AND ]
     %else
       [ WHERE ]
     %end
     (  @{from-extension} ) [ IS FALSE ]
  %end

%else
    %if @{attribs} %then
      [SELECT oid, nspname AS name, nspacl AS permissions, nspowner AS owner, ]

	(@{comment}) [ AS comment ]

       [ FROM pg_namespace ]

       %if @{last-sys-oid} %then
	 [ WHERE oid ] @{oid-filter-op} $sp @{last-sys-oid} [ OR nspname = 'public' ]
       %end

       %if @{filter-oids} %then
	 %if @{last-sys-oid} %then
	   [ AND ]
	 %else
	   [ WHERE ]
	 %end
	 [ oid IN (] @{filter-oids} )
       %end

      %if @{from-extension} %then
	%if @{last-sys-oid} %or @{filter-oids} %then
	  [ AND ]
	%else
	  [ WHERE ]
	%end
	(  @{from-extension} ) [ IS FALSE ]
      %end

    %end
%end
