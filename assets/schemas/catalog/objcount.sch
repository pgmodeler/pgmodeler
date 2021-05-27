# Catalog query to check the amount of objects in pg_class
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[ SELECT count(oid) AS objcount FROM pg_class 
  WHERE relkind IN ('r','i','S','v','m','c','f','p') ]

%if {last-sys-oid} %then
[ AND oid > ] {last-sys-oid}
%end
