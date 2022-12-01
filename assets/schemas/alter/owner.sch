# SQL definition for object's owner attribute change
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

[ALTER ] 

# Special condition for materialized views
#%if {materialized} %then
#	[MATERIALIZED ]
#%end

{sql-object} $sp {signature} [ OWNER TO ] {owner};

$br [-- ddl-end --] $br
