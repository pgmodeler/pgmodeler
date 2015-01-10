# Catalog queries to retrive object comments
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[SELECT description FROM ]

%if {shared-obj} %then
  pg_shdescription
%else
  pg_description
%end

[ WHERE objoid = ] {oid}

%if %not {shared-obj} %then
 [ AND objsubid = 0 ]
%end
