# Catalog queries for collation
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %and %not @{pgsql90} %then
 [SELECT oid, collname AS name FROM pg_collation]
%else
  %if @{attribs} %and %not @{pgsql90}  %then
      [ SELECT cl.oid, cl.collname AS name, cl.collnamespace AS schema, 
               cl.collowner AS owner, pg_encoding_to_char(cl.collencoding) AS encoding,
               cl.collcollate AS lc_collate, cl.collctype AS lc_ctype, ]
               
      (@{comment}) [ AS comment, ]
      (@{from-extension}) [ AS from_extension_bool ]           
               
      [ FROM pg_collation AS cl ]

      %if @{schema} %then
	[ LEFT JOIN pg_namespace AS ns ON cl.collnamespace = ns.oid ]
      %end

      %if @{filter-oids} %or @{schema} %then
	[ WHERE ]
	  %if @{filter-oids} %then
	   [ cl.oid IN (] @{filter-oids} )

	    %if @{schema} %then
	      [ AND ]
	    %end
	  %end

	  %if @{schema} %then
	   [ ns.nspname = ] '@{schema}'
	  %end
      %end
    %end
%end
