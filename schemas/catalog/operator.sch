# Catalog queries for operators
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT op.oid, oprname || '(' || oprleft::regtype || ',' || oprright::regtype || ')' AS name FROM pg_operator AS op ]

  %if @{schema} %then
    [ LEFT JOIN pg_namespace AS ns ON op.oprnamespace = ns.oid
       WHERE ns.nspname = ] '@{schema}'
  %end

  %if @{last-sys-oid} %then
    %if @{schema} %then
     [ AND ]
    %else
     [ WHERE ]
    %end
     [ op.oid ] @{oid-filter-op} $sp @{last-sys-oid}
  %end

  %if @{from-extension} %then
   %if @{last-sys-oid} %or @{schema} %then
    [ AND ]
   %else
    [ WHERE ]
   %end
  (  @{from-extension} ) [ IS FALSE ]
  %end

%else
    %if @{attribs} %then
      [SELECT op.oid, op.oprname AS name, op.oprnamespace AS schema, op.oprowner AS owner,
	      op.oprcanmerge AS merges_bool, op.oprcanhash AS hashes_bool, op.oprleft AS left_type,
	      op.oprright AS right_type, op.oprcom AS commutator_op, op.oprnegate AS negator_op,
	      op.oprrest::oid AS restriction_func, op.oprjoin::oid AS join_func,
	      op.oprcode::oid AS operator_func, ]

      (@{comment}) [ AS comment ]

      [ FROM pg_operator AS op ]

      %if @{schema} %then
	[ LEFT JOIN pg_namespace AS ns ON op.oprnamespace = ns.oid ]
      %end

      %if @{filter-oids} %or @{schema} %then
	[ WHERE ]
	  %if @{filter-oids} %then
	   [ op.oid IN (] @{filter-oids} )

	    %if @{schema} %then
	      [ AND ]
	    %end
	  %end

	  %if @{schema} %then
	   [ ns.nspname = ] '@{schema}'
	  %end
      %end

      %if @{last-sys-oid} %then
	%if @{schema}  %or @{filter-oids} %then
	  [ AND ]
	%else
	  [ WHERE ]
	%end
	[ op.oid ] @{oid-filter-op} $sp @{last-sys-oid}
      %end

      %if @{from-extension} %then
	%if @{last-sys-oid} %or @{schema} %or @{filter-oids} %then
	  [ AND ]
	%else
	  [ WHERE ]
	%end
	(  @{from-extension} ) [ IS FALSE ]
      %end

    %end
%end
