# pt_BR:
# Definição XML para tipos definidos pelo usuario
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for user defined types
# CAUTION: Do not modify this file unless you know what
#          you are doing.
[<usertype name=] "@{name}"

$sp
%if @{base} %then configuration="base" %end
%if @{composite} %then configuration="composite" %end
%if @{enumeration} %then configuration="enumeration" %end

%if @{base} %then
 $br
 $tb $tb [ internal-length=] "@{internal-length}"
 [ by-value=] %if @{by-value} %then "true" %else "false" %end
 $br

 $tb $tb [ alignment=] "@{alignment}"
 [ storage=] "@{storage}"
 $br

 %if @{element} %then $tb $tb [ element=] "@{element}" %end
 %if @{delimiter} %then [ delimiter=] "@{delimiter}" %end
 $br

 %if @{default-value} %then
  $tb $tb [ default-value=] "@{default-value}"
 %end

 %if @{category} %then
  $tb $tb [ category=] "@{category}"
 %end

 %if @{preferred} %then 
  $tb $tb [ preferred=] "true"
 %end
%end


 %if @{protected} %then 
  [ protected=] "true"
 %end

 > $br

 @{schema}
 %if @{owner} %then @{owner} %end
 %if @{comment} %then @{comment} %end

 %if @{enumeration} %then
   %if @{enumerations} %then 
     $tb [<enumeration values=] "@{enumerations}" /> $br
   %end
 %end

 %if @{elements} %then  @{elements} %end
 
 %if @{base} %then  
   %if @{like-type} %then
     [<type name=] "@{like-type}" /> $br
   %end

   %if @{input} %then @{input} %end 
   %if @{output} %then @{output} %end
   %if @{receive} %then @{receive} %end
   %if @{send} %then @{send} %end
   %if @{tpmodin} %then @{tpmodin} %end
   %if @{tpmodout} %then @{tpmodout} %end
   %if @{analyze} %then @{analyze} %end
 %end

</usertype> $br $br