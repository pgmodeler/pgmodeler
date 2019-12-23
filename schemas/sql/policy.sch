# SQL definition for policy
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} >=f "9.5") %then
    [-- object: ] {name} [ | type: ] {sql-object} [ --] $br
    [-- ] {drop}

    %if {prepended-sql} %then
        {prepended-sql}
        $br [-- ddl-end --] $br $br
    %end

    [CREATE POLICY ] {name} [ ON ] {table}

    %if ({pgsql-ver} >=f "10.0") %then
        %if {permissive} %then
            $br $tb [AS PERMISSIVE]
        %else
            $br $tb [AS RESTRICTIVE]
        %end
    %end

    $br $tb [FOR ] {command}
    $br $tb [TO ] 
    
    %if {roles} %then 
        {roles}
    %else
        PUBLIC
    %end
        
    %if {using-exp} %then
        $br $tb [USING (] {using-exp} [)]
    %end

    %if {check-exp} %then
        $br $tb [WITH CHECK (] {check-exp} [)]
    %end

    ;$br

    # This is a special token that pgModeler recognizes as end of DDL command
    # when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
    [-- ddl-end --] $br

    %if {comment} %then {comment} %end

    %if {appended-sql} %then
        {appended-sql}
        $br [-- ddl-end --] $br
    %end

    $br
%end
