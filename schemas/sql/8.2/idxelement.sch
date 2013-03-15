# SQL definition for operator class elements
# PostgreSQL Version: 8.2
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
%if @{column} %then
 @{column}
%else 
  %if @{expression} %then
    [(] @{expression} [)]
  %end
%end
