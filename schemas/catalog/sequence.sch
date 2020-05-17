# Catalog queries for sequences
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
  [SELECT sq.oid, relname AS name, ns.nspname AS parent, 'schema' AS parent_type
   FROM pg_class AS sq 
   LEFT JOIN pg_namespace AS ns ON sq.relnamespace = ns.oid ]

  %if {schema} %then
    [ 
       WHERE relkind='S' AND ns.nspname = ] '{schema}'
  %else
    [ WHERE relkind='S']
  %end

  %if {last-sys-oid} %then
     [ AND sq.oid ] {oid-filter-op} $sp {last-sys-oid}
  %end

  %if {not-ext-object} %then
    [ AND ] (  {not-ext-object} )
  %end
  
  %if {name-filter} %then
    [ AND ] ( {name-filter} )
  %end

%else
    %if {attribs} %then
      #Creates a temporary function to retrieve the sequence attributes
      [CREATE OR REPLACE FUNCTION pg_temp.__pgmodeler_tmp_get_seq_attribs(text,text)
       RETURNS text
       AS
       $$
	DECLARE
	 res record;
	BEGIN ]
	
	%if ({pgsql-ver} <=f "9.6") %then
	 [ EXECUTE 'SELECT start_value, min_value, max_value, increment_by, cache_value, ' ||
	 	   ' CASE WHEN is_cycled IS FALSE THEN NULL ' ||
		   ' ELSE  is_cycled END FROM ' || '"' || $1 || '"' || '.' || '"' || $2 || '"' || ' LIMIT 1;' INTO res; ]
	%else
	
	 [ EXECUTE 'SELECT s.seqstart AS start_value, s.seqmin AS min_value, s.seqmax AS max_value, ' ||
                   '       s.seqincrement AS increment_by, seqcache as cache_value, ' ||
                   ' CASE WHEN seqcycle IS FALSE THEN NULL ELSE seqcycle END AS is_cycled ' ||
                   ' FROM pg_sequence AS s ' ||
                   ' LEFT JOIN pg_class AS c ON s.seqrelid = c.oid ' ||
                   ' LEFT JOIN pg_namespace AS n ON n.oid = c.relnamespace ' ||
                   ' WHERE n.nspname = ''' || $1 ||''' AND c.relname = ''' || $2 ||''' LIMIT 1;' INTO res; ]
	%end
		 
		 
        [   RETURN replace(replace(res::text,'(','{'),')','}');
	END
       $$
       LANGUAGE plpgsql; ]

      [ SELECT sq.oid, sq.relname AS name, sq.relnamespace AS schema, sq.relowner AS owner,
	  (SELECT DISTINCT refobjid || ':' || refobjsubid FROM pg_depend WHERE classid = 'pg_class'::regclass::oid AND objid=sq.oid AND deptype in ('a', 'i')) AS owner_col,
	  (pg_temp.__pgmodeler_tmp_get_seq_attribs(ns.nspname, sq.relname)) AS attribute, ]

      ({comment}) [ AS comment ]

      [ FROM pg_class AS sq
	 LEFT JOIN pg_namespace AS ns ON ns.oid=sq.relnamespace
	 WHERE relkind='S' ]

	%if {last-sys-oid} %then
	  [ AND sq.oid ] {oid-filter-op} $sp {last-sys-oid}
	%end

	%if {not-ext-object} %then
	  [ AND ] ( {not-ext-object} )
	%end

	%if {filter-oids} %or {schema} %then
	[ AND ]
	  %if {filter-oids} %then
	   [ sq.oid IN (] {filter-oids} )

	    %if {schema} %then
	      [ AND ]
	    %end
	  %end

	  %if {schema} %then
	   [ ns.nspname= ] '{schema}'
	  %end
       %end
    %end
%end
