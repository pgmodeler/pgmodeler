# pt_BR:
# Definição XML para arquivo de configurações básicas
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for basic configurations file
# CAUTION: Do not modify this file unless you know what
#          you are doing.
[<?xml version="1.0" encoding="UTF-8" ?>] $br
[<!--] $br
[  ATENÇÃO: Não modifique este arquivo diretamente no código a não ser que você saiba ] $br
[           o que está fazendo. Resultados inesperados podem acontecer] $br
[           caso o código seja alterado deliberadamente.] $br
$br
[  CAUTION: Do not modify this file directly on it's code unless you know what you are doing.] $br
[           Unexpected results may occur if the code is changed deliberately.] $br
[-->] $br

<pgmodeler> $br
$sp [<configuration op-list-size="] @{op-list-size} ["] $br
[		 grid-size="] @{grid-size} ["] $br
[                autosave-interval="] @{autosave-interval} ["] $br
[		paper-type="] @{paper-type} ["] $br
[		paper-orientation="] @{paper-orientation} ["] $br
[		paper-margin="] @{paper-margin} ["] $br
[		save-session="] %if @{save-session} %then true %else false %end ["] $br
[		save-widgets="] %if @{save-widgets} %then true %else false %end ["/>] $br

%if @{widget} %then
 $sp <widget-conf> $br
 $sp @{widget}
 $sp </widget-conf> $br
%end

%if @{file} %then
 $sp <session> $br
 $sp @{file}
 $sp </session> $br
%end

</pgmodeler> $br
