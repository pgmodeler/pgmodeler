# Catalog queries for languages
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT oid, lanname AS name FROM pg_language ]

  %if @{last-sys-oid} %then
   [ WHERE oid ] @{oid-filter-op} $sp @{last-sys-oid}
  %end

  %if @{from-extension} %then
    %if @{last-sys-oid} %then
      [ AND ]
    %else
     [ WHERE ]
    %end
    ( @{from-extension} ) [ IS FALSE ]
  %end

%else
    %if @{attribs} %then
      [SELECT oid, lanname AS name, lanpltrusted AS trusted_bool,
	      lanplcallfoid AS handler, laninline AS inline,
	      lanvalidator AS validator, lanacl AS permissions, lanowner AS owner, ]

      (@{comment}) [ AS comment ]

      [ FROM pg_language ]

       %if @{last-sys-oid} %then
	 [ WHERE oid ] @{oid-filter-op} $sp @{last-sys-oid}
       %end

       %if @{from-extension} %then
	 %if @{last-sys-oid} %then
	   [ AND ]
	 %else
	   [ WHERE ]
	 %end
	 ( @{from-extension} ) [ IS FALSE ]
       %end

       %if @{filter-oids} %then
	 %if @{last-sys-oid} %or @{from-extension} %then
	   [ AND ]
	 %else
	   [ WHERE ]
	 %end
	 [ oid IN (] @{filter-oids} )
       %end
    %end
%end
