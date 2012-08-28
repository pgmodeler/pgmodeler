# pt_BR:
# Definição SQL para sequencias
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for sequences
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE SEQUENCE ] @{name} $br

%if @{increment} %then
 $tb [INCREMENT BY ] @{increment} $br
%end

%if @{min-value} %then
 $tb [MINVALUE ] @{min-value} $br
%end

%if @{max-value} %then
 $tb [MAXVALUE ] @{max-value} $br
%end

%if @{start} %then
 $tb [START WITH ] @{start} $br
%end

%if @{cache} %then
 $tb [CACHE ] @{cache} $br
%end

%if @{cycle} %then
  $tb CYCLE
%else
  $tb [NO CYCLE]
%end
$br

$tb [OWNED BY ]
%if @{owner} %then @{owner} %else NONE %end
; $br

%if @{comment} %then @{comment} %end
