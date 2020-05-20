# Catalog queries for foreign data wrappers
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
    [ SELECT fw.oid, fdwname AS name, current_database() AS parent, 'database' AS parent_type
      FROM pg_foreign_data_wrapper AS fw ]

    %if {last-sys-oid} %or {not-ext-object} %or {name-filter} %then  
        [ WHERE ]
        
        %if {last-sys-oid} %then
            [ fw.oid ] {oid-filter-op} $sp {last-sys-oid}
        %end

        %if {not-ext-object} %then
            
            %if {last-sys-oid} %then
                [ AND ] 
            %end
            
            ( {not-ext-object} ) 
        %end
        
        %if {name-filter} %then
            %if {last-sys-oid} %or {not-ext-object} %then
                [ AND ] 
            %end
           
            ( {name-filter} )
        %end  
    %end
%else 
    %if {attribs} %then
        [SELECT oid, fdwname AS name, ]
            
        %if ({pgsql-ver} >=f "9.1") %then
            [ fdwhandler AS handler, ]
        %else
            [ NULL AS handler, ]
        %end            
            
        [ fdwvalidator AS validator, 
          fdwacl AS permission, fdwowner AS owner, fdwoptions AS options, ]

        ({comment}) [ AS comment ]

        [ FROM pg_foreign_data_wrapper AS fw ]
        
        %if {last-sys-oid} %then
            [ WHERE oid ] {oid-filter-op} $sp {last-sys-oid}
        %end

        %if {not-ext-object} %then
            %if {last-sys-oid} %then
                [ AND ]
            %else
                [ WHERE ]
            %end
	 
            ( {not-ext-object} )
        %end

        %if {filter-oids} %then
            %if {last-sys-oid} %or {not-ext-object} %then
                [ AND ]
            %else
                [ WHERE ]
            %end
            
            [ oid IN (] {filter-oids} )
        %end
    %end
%end
