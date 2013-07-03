# Catalog queries to retrive the owner (role) associated to a object
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[SELECT rolname AS name FROM pg_roles WHERE oid = ] @{oid}
