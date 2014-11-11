# SQL definition for table attributes change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{oids} %or @{without-oids} %then
  [ALTER TABLE ] @{name} [ SET ]
  
  %if @{oids} %then
    WITH
  %else
    WITHOUT
  %end
  
  [ OIDS];
  $br [-- ddl-end --] $br
%end

%if @{inherit} %or @{no-inherit} %then
  [ALTER TABLE ] @{name} 
  
  %if @{inherit} %then
   [ NO ]
  %end
  
  [INHERIT ] @{ancestor-table};
  $br [-- ddl-end --] $br
%end
