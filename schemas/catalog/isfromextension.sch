# Catalog query to check if the object was created by an exentsion
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

# If the query return at least one result indicates that the object is part
# of a extension and it will not reverse engineered, instead, it's parent extension will
[SELECT objid FROM pg_depend ]
[ WHERE (objid = ] @{oid} [ OR refobjid = ] @{oid} )
[ AND deptype IN ('e') ]
