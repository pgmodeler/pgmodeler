# pt_BR:
# Definição XML para relacionamentos
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for relationships
# CAUTION: Do not modify this file unless you know what
#          you are doing.
[<relationship name=] "@{name}" $br
$tb
[ type=] "@{type}"

 %if @{protected} %then 
  [ protected=] "true"
 %end $br

$tb
[ src-required=] %if @{src-required} %then "true" %else "false" %end
[ dst-required=] %if @{dst-required} %then "true" %else "false" %end $br

%if @{src-sufix} %then
 $tb [ src-sufix=] "@{src-sufix}" $br
%end

%if @{dst-sufix} %then
 $tb [ dst-sufix=] "@{dst-sufix}" $br
%end

%if @{identifier} %then 
 $tb $sp identifier="true" $br
%end

%if @{table-name} %then 
 $tb $sp table-name="@{table-name}" $br
%end

%if @{deferrable} %then 
 $tb $sp deferrable="true" 
 $sp defer-type="@{defer-type}" $br
%end

$tb [ src-table=] "@{src-table}" $br
$tb [ dst-table=] "@{dst-table}"

%if @{reduced-form} %then
 /> $br $br
%else
 > $br

 %if @{points} %then $tb <line> $br $tb @{points} $tb </line> $br %end
 %if @{labels-pos} %then @{labels-pos} %end

 %if @{columns} %then @{columns} %end
 %if @{constraints} %then @{constraints} %end
 
 %if @{special-pk-cols} %then 
  $tb [<special-pk-cols indexes=]"@{special-pk-cols}"/> $br
 %end

 </relationship>  $br $br
%end