# pt_BR:
# Definição XML para configuração de conexões
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for connections configuration
# CAUTION: Do not modify this file unless you know what
#          you are doing.
%if @{connections} %then

[<?xml version="1.0" encoding="UTF-8" ?>] $br
[<!--] $br
[  ATENÇÃO: Não modifique este arquivo diretamente no código a não ser que você saiba ] $br
[           o que está fazendo. Resultados inesperados podem acontecer] $br
[           caso o código seja alterado deliberadamente.] $br
$br
[  CAUTION: Do not modify this file directly on it's code unless you know what you are doing.] $br
[           Unexpected results may occur if the code is changed deliberately.] $br
[-->] $br

<connections> $br
@{connections} $br
</connections> $br

%else

$tb [<connection alias="] @{alias} [" host=] "@{host}" [ port=] "@{port}" [ dbname=] "@{dbname}" $br
$tb $tb [user=] "@{user}" [ password=] "@{password}" [ connect_timeout=] "@{connect_timeout}" $br
$tb $tb [sslmode=] "@{sslmode}" $br

%if @{sslcert} %then $tb $tb [sslcert=] "@{sslcert}" $br %end
%if @{sslkey} %then  $tb $tb [sslkey=] "@{sslkey}" $br %end 
%if @{sslrootcert} %then $tb $tb [sslrootcert=] "@{sslrootcert}" $br %end 
%if @{sslcrl} %then $tb $tb [sslcrl=] "@{sslcrl}" $br %end


%if @{krbsrvname} %then
 $tb $tb [krbsrvname=] "@{krbsrvname}" 

 %if @{gsslib} %then 
   [ gsslib=] "@{gsslib}"
 %end
 $br 
%end

%if @{options} %then
 $tb $tb options="@{options}"
%end

 $tb $tb /> $br
%end