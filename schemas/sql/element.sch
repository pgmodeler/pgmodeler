# SQL definition for operator class elements
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
%if @{function} %then
 $tb FUNCTION $tb @{stg-number} $tb @{signature}
%end

%if @{operator} %then
 $tb OPERATOR $tb @{stg-number} $tb @{signature}
 %if @{recheck} %then
   $tb RECHECK
 %end
%end

%if @{storage} %then
 $tb STORAGE $tb @{type}
%end