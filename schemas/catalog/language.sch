# Catalog queries for languages
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

%if @{list} %then
  # Built-in languages aren't shown: internal, C, SQL, plpgsql
  [SELECT oid, lanname AS name FROM pg_language
    WHERE lanname <> 'plpgsql' AND lanispl IS TRUE ]
%else
    %if @{attribs} %then

SELECT oid, lanname AS name, lanpltrusted AS trusted_bool,
       lanplcallfoid::regproc AS handler_func, laninline::regproc AS inline_func,
       lanvalidator::regproc AS validator_func, lanacl AS permissions  FROM pg_language




     WHERE  lanispl IS TRUE
    %end
%end
