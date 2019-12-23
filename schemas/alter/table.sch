# SQL definition for table's attributes change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

{alter-cmds}

%if {has-changes} %then
  %set {alter}   ALTER $sp {sql-object} $sp {signature}
  %set {rls}     [ROW LEVEL SECURITY]
  %set {ddl-end} ; $br [-- ddl-end --] $br

  %if ({pgsql-ver} <=f "11.0") %and {oids} %then
    {alter} [ SET ]

    %if ({oids}=="unset") %then
      WITHOUT
    %else
      WITH
    %end

    [ OIDS]
    {ddl-end}
  %end

  %if {unlogged} %then
    {alter} [ SET ]

    %if ({unlogged}=="unset") %then
      [LOGGED]
    %else
      [UNLOGGED]
    %end

    {ddl-end}
  %end

  %if {rls-enabled} %then
    {alter} 

    %if ({rls-enabled}=="unset") %then
      [ DISABLE ]
    %else
      [ ENABLE ]
    %end

    {rls}
    {ddl-end}
  %end

  %if {rls-forced} %then
    {alter}

    %if ({rls-forced}=="unset") %then
      [ NO]
    %end

    [ FORCE ] {rls}
    {ddl-end}
  %end

%end
