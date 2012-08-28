# pt_BR:
# Definição XML para funções
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for functions
# CAUTION: Do not modify this file unless you know what
#          you are doing.
%if @{reduced-form} %then $tb %end

<function

%if @{reduced-form} %then
  %if @{ref-type} %then 
    [ ref-type=] "@{ref-type}"
  %end
  [ signature=] "@{signature}"
%else
  [ name=] "@{name}"
%end

%if @{reduced-form} %then
/> $br
%else

  %if @{protected} %then 
   [ protected=] "true"
  %end

  $br
  $tb $tb window-func=%if @{window-func} %then "true" %else "false" %end $br
  $tb $tb returns-setof=%if @{returns-setof} %then "true" %else "false" %end $br
  $tb $tb behavior-type="@{behavior-type}" $br
  $tb $tb function-type="@{function-type}" $br
  $tb $tb security-type="@{security-type}" $br
  $tb $tb execution-cost="@{execution-cost}" $br
  $tb $tb row-amount="@{row-amount}"
  > $br

  @{schema}
  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end
  %if @{language} %then @{language} %end

  $tb <return-type> $br
       %if @{return-table} %then
          @{return-table}
       %else
          @{return-type}
        %end
  $tb </return-type> $br
  
  %if @{parameters} %then @{parameters} %end


  %if @{library} %then
   $tb [<definition library=] "@{library}"

   %if @{symbol} %then
     [ symbol=] "@{symbol}"
   %end

    /> $br

  %else
   $tb <definition>@{definition}</definition> $br
  %end

  </function> $br $br
%end