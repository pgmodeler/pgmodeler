# SQL definition for domain's attributes change
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

#Changing default value
%if @{default-value} %then
  [ALTER ] @{sql-object} $sp @{signature} 

  %if (@{default-value} == "unset") %then
    [ DROP DEFAULT]
  %else
    [ SET DEFAULT ] @{default-value}
  %end
  ; $br
  [-- ddl-end --] $br
%end

#Changing NOT NULL constraint
%if @{not-null} %then
  [ALTER ] @{sql-object} $sp @{signature} 

  %if (@{not-null} == "unset") %then
    [ DROP ]
  %else
    [ SET ]
  %end
  
  [NOT NULL;] $br
  [-- ddl-end --] $br
%end

#Changing CHECK expression
%if @{expression} %then
  [ALTER ] @{sql-object} $sp @{signature} 

  %if (@{expression} == "unset") %then
    [ DROP CONSTRAINT ]
  %else
    [ ADD CONSTRAINT ]
  %end
  
  %if @{constraint} %then
    @{constraint}
  %end
  
  %if (@{expression} != "unset") %then
    $sp CHECK(@{expression})
  %end
  
  ; $br
  [-- ddl-end --] $br
%end

#Changing constraint name (only in 9.2 or above)
%if (@{pgsql-ver} >= "9.2") %and @{old-name} %and @{new-name} %then
  [ALTER ] @{sql-object} $sp @{signature} 
  [ RENAME CONSTRAINT ] @{old-name} [ TO ] @{new-name} ; $br
  [-- ddl-end --] $br
%end
