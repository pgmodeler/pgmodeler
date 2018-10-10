# Catalog queries for table partition keys
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[SELECT partrelid AS table, 
    CASE partstrat
        WHEN 'l' then 'LIST'
        WHEN 'r' then 'RANGE'
        ELSE 'HASH'
    END AS partitioning,
	pg_get_expr(partexprs, partrelid) AS expressions,
	partattrs::oid] $ob $cb [ AS columns, 
	partclass::oid] $ob $cb [ AS opclasses, 
	partcollation::oid] $ob $cb [ AS collations 
	FROM pg_partitioned_table ]
	
%if {filter-oids} %then
    [ WHERE partrelid IN (] {filter-oids} )
%end
