# Catalog queries for casts
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
   [SELECT oid, 'cast('|| castsource::regtype::text || ',' || casttarget::regtype::text || ')' AS name
     FROM pg_cast ]

  %if @{last-sys-oid} %then
   [ WHERE oid > ] @{last-sys-oid}
  %end

%else
    %if @{attribs} %then
      [SELECT oid, 'cast('|| castsource::regtype::text || ',' || casttarget::regtype::text || ')' AS name,
	      castsource AS source_type, casttarget AS dest_type, castfunc AS function,

	 CASE castcontext
	    WHEN  'a' THEN 'assignment'
	    WHEN  'i' THEN 'implicit'
	    ELSE 'explicit'
	 END AS cast_type,

	 CASE castmethod
	    WHEN  'b' THEN TRUE
	    ELSE FALSE
	 END AS io_cast_bool, ]

      (@{comment}) [ AS comment, ]
      (@{from-extension}) [ AS from_extension_bool ]

     [ FROM pg_cast AS cs]

      %if @{last-sys-oid} %then
       [ WHERE oid > ] @{last-sys-oid}
      %end

    %end
%end
