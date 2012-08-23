# pt_BR:
# Definição SQL para restrições
# Versão PostgreSQL: 8.x, 9.0
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for constraints
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.
%if @{decl-in-table} %then 
 $tb 
%else
[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br
 %if @{table} %then
  [ALTER TABLE ] @{table} [ ADD ]
  %end
%end

[CONSTRAINT ] @{name}
%if @{ck-constr} %then [ CHECK ] (@{expression}) %end
%if @{pk-constr} %then [ PRIMARY KEY ] (@{src-columns}) %end
%if @{uq-constr} %then [ UNIQUE ] (@{src-columns}) %end

%if @{tablespace} %then
  $br
  %if @{decl-in-table} %then $tb %end
  [WITH (FILLFACTOR = ] @{factor} [)] $br
  %if @{decl-in-table} %then $tb %end
  %if @{pk-constr} %then [USING INDEX TABLESPACE ] @{tablespace} %end
  %if @{uq-constr} %then [USING INDEX TABLESPACE ] @{tablespace} %end
%end

%if @{fk-constr} %then
  [ FOREIGN KEY ] (@{src-columns}) $br
  
  %if @{decl-in-table} %then $tb %end
  [REFERENCES ] @{ref-table} $sp (@{dst-columns})
  $sp @{comparison-type} $br
  
  %if @{decl-in-table} %then $tb %end
  [ON DELETE ] @{del-action} [ ON UPDATE ] @{upd-action}

  %if @{deferrable} %then
   [ DEFERRABLE ] @{defer-type}
  %else
   [ NOT DEFERRABLE]
  %end

%end

%if @{decl-in-table} %then [,] 
%else
 [;] $br
%end $br
