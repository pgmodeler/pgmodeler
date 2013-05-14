# SQL definition for user defined types
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- object: ] @{name} [ | type: ] @{sql-object} [ --] $br

[CREATE TYPE ] @{name}

%if @{reduced-form} %then
; $br
%else
   %if @{base} %then 
     [ (] $br %else [ AS] $br
   %end

   %if @{composite} %then 
    ( $br @{typeattrib} $br );
   %end

   %if @{enumeration} %then
     %if @{enumerations} %then [ ENUM ] (@{enumerations}); %end
   %end

   %if @{pgsql92} %and @{range} %then
    [SUBTYPE = ] @{subtype}
    %if @{collation} %then  $br [, COLLATION = ] @{collation} %end 
    %if @{opclass} %then  $br [, SUBTYPE_OPCLASS = ] @{opclass} %end
    %if @{canonical} %then  $br [, CANONICAL = ] @{canonical} %end
    %if @{subtypediff} %then  $br [, SUBTYPE_DIFF = ] @{subtypediff} %end
   %end
   
   %if @{base} %then
     %if @{input} %then $tb [INPUT = ] @{input}, $br %end
     %if @{output} %then $tb [OUTPUT = ] @{output} $br %end
     %if @{receive} %then $tb [, RECEIVE = ] @{receive} $br %end
     %if @{send} %then $tb [, SEND = ] @{send} $br %end
     %if @{tpmodin} %then $tb [, TYPMOD_IN = ] @{tpmodin} $br %end
     %if @{tpmodout} %then $tb [, TYPMOD_OUT = ] @{tpmodout} $br %end
     %if @{analyze} %then $tb [, ANALYZE = ] @{analyze} $br %end
     %if @{internal-length} %then $tb [, INTERNALLENGTH = ] @{internal-length} $br %end
     %if @{by-value} %then $tb [, PASSEDBYVALUE ] $br %end
     %if @{alignment} %then $tb [, ALIGNMENT = ] @{alignment} $br %end
     %if @{storage} %then $tb [, STORAGE = ] @{storage} $br %end
     %if @{default-value} %then $tb [, DEFAULT = ] @{default-value} $br %end
     %if @{element} %then $tb [, ELEMENT = ] @{element} $br %end
     %if @{delimiter} %then $tb [, DELIMITER = ] '@{delimiter}' $br %end
     %if @{like-type} %then $tb [, LIKE = ] @{like-type} $br %end
     %if @{category} %then $tb [, CATEGORY = ] '@{category}' $br %end
     %if @{preferred} %then $tb [, PREFERRED = ] true $br %end
     %if @{collatable} %then $tb [, COLLATABLE = ] true $br %end

    );
  %end
  $br
  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end
%end

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
[-- ddl-end --] $br $br
