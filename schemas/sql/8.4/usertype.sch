# pt_BR:
# Definição SQL para tipos definidos pelo usuario
# Versão PostgreSQL: 8.4
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for user defined types
# PostgreSQL Version: 8.4
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE TYPE ] @{name}

%if @{reduced-form} %then
; $br
%else
   %if @{base} %then [ (] $br %else $br AS %end

   # Configuração para tipo composto
   %if @{composite} %then $sp (@{elements}); %end

   # Configuração para tipo enumeração
   %if @{enumeration} %then
     %if @{enumerations} %then [ ENUM ] (@{enumerations}); %end
   %end

   # Configuração para tipo base
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

    );
  %end
  $br
  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end
%end
