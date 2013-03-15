# SQL definition for relationships
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{rel1n} %then
 @{constraints}
%end

%if @{relgen} %then
 [ALTER TABLE ] @{table} [ INHERIT ] @{ancestor-table}; $br $br
%end

%if @{relnn} %then
 @{table}
 @{constraints}
%end

