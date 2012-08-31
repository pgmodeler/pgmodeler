# pt_BR:
# Definição XML para papéis
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# XML definition for roles
# CAUTION: Do not modify this file unless you know what
#          you are doing.
%if @{reduced-form} %then $tb %end
[<role name=] "@{name}"

%if @{reduced-form} %then
/> $br
%else
 $tb
 %if @{sysid} %then [ sysid=] "@{sysid}" %end
 %if @{superuser} %then [ superuser="true"]  %end
 %if @{createdb} %then [ createdb="true"] %end
 $br

 $tb
 %if @{createrole} %then [ createrole="true"] %end
 %if @{inherit} %then [ inherit="true"] %end
 %if @{login} %then [ login="true"] %end
 $br

 $tb
 %if @{encrypted} %then [ encrypted="true"] %end
 %if @{connlimit} %then [ connlimit=] "@{connlimit}" %end
 %if @{validity} %then [ validity=] "@{validity}" %end
 $br

 $tb
 %if @{password} %then [ password=] "@{password}" %end
 %if @{protected} %then 
  [ protected=] "true"
 %end

 > $br

 %if @{comment} %then @{comment} $br %end

 %if @{ref-roles} %then $tb [<roles names=] "@{ref-roles}" [ role-type="refer" />] $br %end
 %if @{member-roles} %then $tb [<roles names=] "@{member-roles}" [ role-type="member" />] $br %end
 %if @{admin-roles} %then $tb [<roles names=] "@{admin-roles}" [ role-type="admin" />] $br %end



 </role>  $br $br
%end