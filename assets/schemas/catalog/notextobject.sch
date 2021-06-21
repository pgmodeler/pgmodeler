# Catalog query to check if the object was created by an exentsion
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {ext-obj-oids} %then
	{oid} [ NOT IN ( ] {ext-obj-oids} [) ]
%end

