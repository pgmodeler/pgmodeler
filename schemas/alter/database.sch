# SQL definition for database's attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {connlimit} %then
  [ALTER ] {sql-object} $sp {signature} [ WITH ] 

  %if {connlimit} %then
    $br $tb [ CONNECTION LIMIT ] {connlimit} 
  %end  

  ; $br

  [-- ddl-end --] $br
%end

%if ({pgsql-ver} >=f "9.5") %then

  %if {allow-conns} %or {is-template} %then

    [ALTER ] {sql-object} $sp {signature} [ WITH ] 
  
    %if {allow-conns} %then
      $br $tb [ ALLOW_CONNECTIONS ] {allow-conns} 
    %end

    %if {is-template} %then
      $br $tb [ IS_TEMPLATE ] {is-template} 
    %end

    ; $br

    [-- ddl-end --] $br

  %end

%end
