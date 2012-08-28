# pt_BR:
# Definição SQL para permissões em objetos
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for permissions on objects
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.
# 

[-- object: ] @{name} [ | type: ] PERMISSION [ -- ]

%if @{privileges} %then

  $br [GRANT ] @{privileges} $br

  %if @{parent} %then
    $sp $sp (@{object}) [ ON TABLE ] @{parent} $br
  %else
    $sp $sp  [ ON ] @{type} $sp @{object} $br
  %end

   $sp $sp  [ TO ] 

  %if @{roles} %then   
    @{roles}
  %else 
    PUBLIC
  %end
  ; $br

%end


%if @{privileges-gop} %then

  $br [GRANT ] @{privileges-gop} $br

  %if @{parent} %then
    $sp $sp (@{object}) [ ON TABLE ] @{parent} $br
  %else
    $sp $sp  [ ON ] @{type} $sp @{object} $br
  %end
   $sp $sp  [ TO ]  @{roles} [ WITH GRANT OPTION]; $br

%end
