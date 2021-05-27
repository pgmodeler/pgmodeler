# SQL definition for transforms
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} >=f "9.5") %then
    [-- object: ] {name} [ | type: ] {sql-object} [ --] $br
    [-- ] {drop}

    # This is a special token that pgModeler recognizes as end of DDL command
    # when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
    %set {ddl-end} $br [-- ddl-end --] $br

    %if {prepended-sql} %then
        {prepended-sql}
        {ddl-end} $br
    %end

    [CREATE TRANSFORM FOR ] {type} [ LANGUAGE ] {language} [ (]

    # One of the two function should be defined otherwise the code generation will fail
    %if {fromsql} %or %not {tosql} %then
        $br $tb [FROM SQL WITH FUNCTION ] {fromsql}
    %end

    %if {fromsql} %and {tosql} %then
        ,
    %end

    %if {tosql} %or %not {fromsql} %then
        $br $tb [TO SQL WITH FUNCTION ] {tosql} 
    %end

    $br [);] 

    {ddl-end}

    %if {comment} %then 
        {comment} 
    %end

    %if {appended-sql} %then
        {appended-sql}
        {ddl-end}
    %end

    $br
%end
