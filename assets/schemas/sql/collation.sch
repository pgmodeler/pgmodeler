# SQL definition for collations
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if ({pgsql-ver} != "9.0") %then

    [-- object: ] {name} [ | type: ] {sql-object} [ --] $br
    [-- ] {drop}

    # This is a special token that pgModeler recognizes as end of DDL command
    # when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
    %set {ddl-end} $br [-- ddl-end --] $br
    %set {attr-sep} [,] $br

    %if {prepended-sql} %then
        {prepended-sql}
        {ddl-end} $br
    %end

    [CREATE COLLATION ] {name}

    %if {collation} %then
        [ FROM ] {collation}
    %else
        [ (] $br
        
        %if {locale} %then
            [   LOCALE = '] 
            
            {locale} 
            
            %if {locale-mod} %then
                @ {locale-mod}
            %end
            
            [']            
        %else
            %if {lc-ctype} %then
                [   LC_CTYPE = '] 
                
                {lc-ctype} 
                
                %if {lc-ctype-mod} %then
                    @ {lc-ctype-mod}
                %end
                
                [']
            %end

            %if {lc-collate} %then
                %if {lc-ctype} %then {attr-sep} %end
                [   LC_COLLATE = '] 
                
                {lc-collate} 
                
                %if {lc-collate-mod} %then
                    @ {lc-collate-mod}
                %end
                
                [']
            %end
        %end

        %if ({pgsql-ver} >=f "10.0") %then
            %if {provider} %then
                %if {locale} %or {lc-collate} %or {lc-ctype} %then {attr-sep} %end
                [   PROVIDER =] '{provider}'
            %end
        %end

        %if ({pgsql-ver} >=f "12.0") %then
            %if {locale} %or {lc-collate} %or {lc-ctype} %or {provider} %then {attr-sep} %end
            [   DETERMINISTIC = ] {deterministic}
        %end
        
        $br [)]
    %end

    ; 

    {ddl-end}

    %if {owner} %then {owner} %end
    %if {comment} %then {comment} %end

    %if {appended-sql} %then
        {appended-sql}
        {ddl-end}
    %end
    $br
%end
