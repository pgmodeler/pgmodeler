# SQL definition for collation change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if %not @{pgsql90} %then
 @{alter-cmds}
%end
