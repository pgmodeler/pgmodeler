# pt_BR:
# Definição XML para referencias de visoes
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for view references
# CAUTION: Do not modify this file unless you know what
#          you are doing.
$tb <reference


#%if @{view} %then
#  [ view=] "@{view}" 
#%end

%if @{table} %then
  [ table=] "@{table}" 
  %if @{column} %then [ column=] "@{column}" %end
  %if @{alias} %then [ alias=] "@{alias}" %end
  %if @{column-alias} %then [ column-alias=] "@{column-alias}" %end
%else
  [ alias=] "@{alias}"
%end

%if @{expression} %then
 > $br
 $tb $tb <expression> @{expression} </expression> $br
$tb </reference> $br
%else
 /> $br
%end
