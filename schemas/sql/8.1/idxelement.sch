# SQL definition for operator class elements
# PostgreSQL Version: 8.1
# CAUTION: Do not modify this file unless that you know what
#          you are doing.
%if @{column} %then
 @{column}
%else 
  %if @{expression} %then
    [(] @{expression} [)]
  %end
%end
