# pt_BR:
# Definição SQL para funções
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for functions
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE OR REPLACE FUNCTION ] $br
$tb @{name} $sp ( %if @{parameters} %then @{parameters} %end ) $br
$tb [RETURNS ]

%if @{return-table} %then
 [TABLE ] ( @{return-table} )
%else
 %if @{returns-setof} %then [SETOF ] %end
 @{return-type}
%end
$br


$tb [LANGUAGE ] @{language} $br

%if @{window-func} %then
 $tb WINDOW $br
%end

$tb @{function-type} $br
$tb @{behavior-type} $br
$tb @{security-type} $br
$tb [COST ] @{execution-cost} $br

%if @{returns-setof} %then
 $tb [ROWS ] @{row-amount} $br
%end

$tb [AS ]

%if @{library} %then
 '@{library}'

 %if @{symbol} %then
   [, ] '@{symbol}'
 %end

%else
[$$]
 @{definition}
[$$]
%end

; $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end
