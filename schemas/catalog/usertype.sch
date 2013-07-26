# Catalog queries for user defined types
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  [SELECT tp.oid, tp.typname AS name FROM pg_type AS tp ]

  %if @{schema} %then
    [ LEFT JOIN pg_namespace AS ns ON tp.typnamespace = ns.oid
       WHERE ns.nspname = ] '@{schema}' [ AND ]
  %else
    [ WHERE ]
  %end

  [ typtype IN ('b','c','e','r')]

%else
    %if @{attribs} %then


    %end
%end
