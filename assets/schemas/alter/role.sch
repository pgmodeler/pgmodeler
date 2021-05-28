# SQL definition for role's attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

{alter-cmds}

%if {has-changes} %then
  [ALTER ] {sql-object} $sp {signature} 
  
  %if {superuser} %then
    $br $tb    
    %if ({superuser}=="unset") %then NO %end
    SUPERUSER
  %end

  %if {createdb} %then 
    $br $tb 
    %if ({createdb}=="unset") %then NO %end
    CREATEDB 
  %end
  
  %if {createrole} %then 
    $br $tb 
    %if ({createrole}=="unset") %then NO %end
    CREATEROLE 
  %end
  
  %if {inherit} %then 
    $br $tb 
    %if ({inherit}=="unset") %then NO %end
    INHERIT 
  %end
  
  %if {login} %then 
    $br $tb 
    %if ({login}=="unset") %then NO %end
    LOGIN 
  %end
  
  %if {replication} %and ({pgsql-ver} != "9.0") %then 
    $br $tb 
    %if ({replication}=="unset") %then NO %end
    REPLICATION 
  %end
  
  %if {bypassrls} %and ({pgsql-ver} >=f "9.5") %then 
    $br $tb 
    %if ({bypassrls}=="unset") %then NO %end
    BYPASSRLS 
  %end
  
  %if {password} %or {empty-password} %then
    $br $tb
    
    %if {empty-password} %then
        [PASSWORD ] ''
    %else
        %if {encrypted} %then
            %if ({encrypted}=="unset") %then 
            [UNENCRYPTED ]
            %else
            [ENCRYPTED ]
            %end    
        %end
        
        [PASSWORD ] '{password}'
    %end
  %end

  %if {connlimit} %then $br $tb [CONNECTION LIMIT ] {connlimit} %end
  %if {validity} %then $br $tb [VALID UNTIL ] '{validity}' %end
  
  ; 
  
  $br [-- ddl-end --] $br
%end
