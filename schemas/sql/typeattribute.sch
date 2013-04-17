# SQL definition for user defined type attributes
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[  ] @{name} $sp @{type}

%if %not @{pgsql90} %and @{collation} %then
[ COLLATION ] @{collation}
%end

, $br
