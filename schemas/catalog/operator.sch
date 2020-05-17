# Catalog queries for operators
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [SELECT op.oid, oprname || '(' || 
  
    CASE 
        WHEN oprleft::char = '0' THEN 'NONE'
        ELSE oprleft::regtype::varchar
    END
  
    || ',' || 
  
    CASE 
        WHEN oprright::char = '0' THEN 'NONE'
        ELSE oprright::regtype::varchar
    END
        
    || ')' AS name, 
    
    ns.nspname AS parent, 'schema' AS parent_type
    
    FROM pg_operator AS op 
    LEFT JOIN pg_namespace AS ns ON op.oprnamespace = ns.oid ]

  %if {schema} %then
    [ WHERE oprcode > 0 AND ns.nspname = ] '{schema}'
  %else
    [ WHERE oprcode > 0 ]
  %end

  %if {last-sys-oid} %then
    [ AND op.oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if {not-ext-object} %then
    [ AND ] ( {not-ext-object} )
  %end
  
  %if {name-filter} %then
    [ AND ] ( {name-filter} )
  %end

%else
    %if {attribs} %then
      [SELECT op.oid, op.oprname AS name, op.oprnamespace AS schema, op.oprowner AS owner,
	      op.oprcanmerge AS merges_bool, op.oprcanhash AS hashes_bool, op.oprleft AS left_type,
	      op.oprright AS right_type, op.oprcom AS commutator_op,
	      op.oprrest::oid AS restriction, op.oprjoin::oid AS "join",
	      op.oprcode::oid AS operfunc, op.oprnegate AS negator_op, ]

      # This case statement selects the correct negator operator for the current operator
      # [ CASE
#	 WHEN op.oprnegate > 0 THEN
#	  (SELECT _op2.oid FROM pg_operator AS _op1
#	     LEFT JOIN pg_operator AS _op2 ON _op1.oprname=_op2.oprname AND _op1.oprnamespace=_op2.oprnamespace
#	     WHERE _op1.oid=op.oprnegate AND _op2.oprcode > 0
#	       AND _op2.oprnegate=0 AND _op2.oprcom=0 AND _op2.oprrest=0 AND _op2.oprjoin=0
#	       AND ((_op2.oprleft > 0 AND _op2.oprright=0) OR (_op2.oprleft=0 AND _op2.oprright > 0)))
#	 ELSE 0
#       END
#       AS negator_op, ]

      ({comment}) [ AS comment ]

      [ FROM pg_operator AS op ]

      %if {schema} %then
	[ LEFT JOIN pg_namespace AS ns ON op.oprnamespace = ns.oid ]
      %end

      [ WHERE oprcode > 0 ]

      %if {filter-oids} %then
	[ AND op.oid IN (] {filter-oids} )
      %end

      %if {schema} %then
	[ AND ns.nspname = ] '{schema}'
      %end

      %if {last-sys-oid} %then
	[ AND op.oid ] {oid-filter-op} $sp {last-sys-oid}
      %end

      %if {not-ext-object} %then
	[ AND (] {not-ext-object} )
      %end

    %end
%end
