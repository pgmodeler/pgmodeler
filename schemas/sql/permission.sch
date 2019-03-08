# SQL definition for permissions on objects
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] {name} [ | type: ] PERMISSION [ --]

%if {privileges} %then

  $br

  %if {revoke} %then
    [REVOKE ]
  %else
    [GRANT ]
  %end

  {privileges} $br

  %if {parent} %then
    $sp $sp  [ ON TABLE ] {parent} $br
  %else
    $sp $sp  [ ON ] {type} $sp {object} $br
  %end

  $sp $sp  %if {revoke} %then [ FROM ] %else [ TO ] %end

  %if {roles} %then
    {roles}

    %if {revoke} %and {cascade} %then
     [ CASCADE]
    %end
  %else
    PUBLIC
  %end

  ; $br
%end


%if {privileges-gop} %then

  $br

  %if {revoke} %then
    [REVOKE GRANT OPTION FOR ]
  %else
    [GRANT ]
  %end

  {privileges-gop} $br

  %if {parent} %then
    $sp $sp  [ ON TABLE ] {parent} $br
  %else
    $sp $sp  [ ON ] {type} $sp {object} $br
  %end

  $sp $sp

  %if {revoke} %then
   [ FROM ] {roles}
   %if {cascade} %then [ CASCADE] %end ; $br
  %else
   [ TO ] {roles} [ WITH GRANT OPTION]; $br
  %end

%end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br
