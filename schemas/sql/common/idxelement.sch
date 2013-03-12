# SQL definition for operator class elements
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless that you know what
#          you are doing.
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
