# Catalog queries for foreign tables
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if {list} %then
    [ SELECT ft.oid, ft.relname AS name FROM pg_class AS ft]

    %if {schema} %then
        [ LEFT JOIN pg_namespace AS ns ON ns.oid=ft.relnamespace
          WHERE ft.relkind IN ('f') AND ns.nspname= ] '{schema}'
    %else
        [ WHERE ft.relkind IN ('f')]
    %end

    %if {last-sys-oid} %then
        [ AND ft.oid ] {oid-filter-op} $sp {last-sys-oid}
    %end

    %if {not-ext-object} %then
        [ AND ] ( {not-ext-object} )
    %end
%else
    %if {attribs} %then


    %end
%end
