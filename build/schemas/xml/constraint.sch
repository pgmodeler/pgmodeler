# pt_BR:
# Definição XML para restrições
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for constraints
# CAUTION: Do not modify this file unless you know what
#          you are doing.

%if @{decl-in-table} %then $tb %end

[<constraint name=] "@{name}" [ type=] "@{type}"

 %if @{fk-constr} %then
   $tb
   %if @{deferrable} %then
    [deferrable=] "true"
    [ defer-type=] "@{defer-type}"
   %end

    [ comparison-type=] "@{comparison-type}"
   $br

   $tb
    [ upd-action=] "@{upd-action}"
    [ del-action=] "@{del-action}"
    [ ref-table=] "@{ref-table}"
 %end

 %if @{factor} %then
  [ factor=] "@{factor}"
 %end

 %if @{protected} %then 
  [ protected=] "true"
 %end
 
 %if @{table} %then 
  [ table=] "@{table}"
 %end

> $br

 %if @{tablespace} %then $tb @{tablespace} %end
 
 %if @{decl-in-table} %then $tb %end
 %if @{src-columns} %then
 $tb <columns  $sp names="@{src-columns}" $sp ref-type="src-columns"/> $br
 %end
 
 %if @{dst-columns} %then
  %if @{decl-in-table} %then $tb %end
  $tb <columns $sp names="@{dst-columns}" $sp ref-type="dst-columns"/> $br
 %end

 %if @{ck-constr} %then
   %if @{expression} %then
    %if @{decl-in-table} %then $tb %end
    $tb <expression> @{expression} </expression> $br
   %end
 %end

 %if @{comment} %then @{comment} %end
 
%if @{decl-in-table} %then $tb %end
</constraint> $br