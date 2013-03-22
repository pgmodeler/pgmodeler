# SQL definition for operator class elements
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
%if @{column} %then
 @{column}
%else 
  %if @{expression} %then
    [(] @{expression} [)]
  %end
%end

%if @{use-sorting} %then
  %if @{asc-order} %then
   [ ASC ]
  %else
   [ DESC ]
  %end

  %if @{nulls-first} %then
   [NULLS FIRST]
  %else
   [NULLS LAST]
  %end
%end
