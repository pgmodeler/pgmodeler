# SQL definition for operator class elements
# PostgreSQL Version: 8.4
# CAUTION: Do not modify this file unless that you know what
#          you are doing.
%if @{function} %then
 $tb FUNCTION $tb @{stg-number} $tb @{signature}
%end

%if @{operator} %then
 $tb OPERATOR $tb @{stg-number} $tb @{signature}
%end

%if @{storage} %then
 $tb STORAGE $tb @{type}
%end