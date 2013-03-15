# SQL definition for permissions on objects
# PostgreSQL Version: 9.0
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] PERMISSION [ -- ]

%if @{privileges} %then

  $br [GRANT ] @{privileges} $br

  %if @{parent} %then
    $sp $sp (@{object}) [ ON TABLE ] @{parent} $br
  %else
    $sp $sp  [ ON ] @{type} $sp @{object} $br
  %end

   $sp $sp  [ TO ] 

  %if @{roles} %then   
    @{roles}
  %else 
    PUBLIC
  %end
  ; $br

%end


%if @{privileges-gop} %then

  $br [GRANT ] @{privileges-gop} $br

  %if @{parent} %then
    $sp $sp (@{object}) [ ON TABLE ] @{parent} $br
  %else
    $sp $sp  [ ON ] @{type} $sp @{object} $br
  %end
   $sp $sp  [ TO ]  @{roles} [ WITH GRANT OPTION]; $br

%end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br