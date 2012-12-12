# SQL definition for relationships
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless that you know what
#          you are doing.

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

