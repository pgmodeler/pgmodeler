# SQL definition for columns
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

$tb @{name} $sp @{type}

%if @{not-null} %then
 [ NOT NULL]
%end

%if @{default-value} %then
 [ DEFAULT ] @{default-value}
%end
,$br
