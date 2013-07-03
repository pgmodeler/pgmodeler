# Catalog queries to retrive the tablespace associated to a object
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

# System tablespaces like pg_default and pg_global aren't listed

[SELECT spcname AS name FROM pg_tablespace ]
[ WHERE oid = ] @{oid} [ AND spcname NOT LIKE 'pg_%']
