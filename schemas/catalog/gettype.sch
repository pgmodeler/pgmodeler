SELECT array_agg(
	CASE 
	  WHEN nspname='pg_catalog' OR
	       nspname='information_schema' OR
	       nspname='pg_toast' THEN
	    typname
	  ELSE
	    nspname || '.' || typname
	  END
	) AS typename FROM pg_type AS tp 
LEFT JOIN pg_namespace AS ns ON tp.typnamespace=ns.oid
WHERE tp.oid=ANY(ARRAY[2275,17,18,16,2163,854,225,603,20812]::oid[])