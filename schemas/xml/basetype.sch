# pt_BR:
# Definição XML para tipos nativos
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for native types
# CAUTION: Do not modify this file unless you know what
#          you are doing.
$tb [<type name=] "@{name}"

%if @{length} %then
 [ length=] "@{length}"
%end

%if @{dimension} %then
 [ dimension=] "@{dimension}"
%end

%if @{precision} %then
 [ precision=] "@{precision}"
%end

%if @{ref-type} %then
 [ ref-type=] "@{ref-type}"
%end

%if @{with-timezone} %then
 [ with-timezone=] "@{with-timezone}"
%end

%if @{interval-type} %then
 [ interval-type=] "@{interval-type}"
%end

/>  $br