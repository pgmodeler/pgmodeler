# pt_BR:
# Definição SQL para operadores
# Versão PostgreSQL: 8.1
# ATENÇÃO: Não modifique este arquivo a não ser que você saiba
#          o que está fazendo.
#
# en_US:
# SQL definition for operators
# PostgreSQL Version: 8.1
# CAUTION: Do not modify this file unless you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

[CREATE OPERATOR ] @{name} [ (]
$br $tb [PROCEDURE = ] @{operfunc}

%if @{left-type} %then
 $br $tb [, LEFTARG = ] @{left-type}
%end

%if @{right-type} %then
 $br $tb [, RIGHTARG = ] @{right-type}
%end

%if @{commutator-op} %then
 $br $tb [, COMMUTATOR = ] OPERATOR(@{commutator-op})
%end

%if @{negator-op} %then
 $br $tb [, NEGATOR = ] OPERATOR(@{negator-op})
%end

%if @{restriction} %then
 $br $tb [, RESTRICT = ] @{restriction}
%end

%if @{join} %then
 $br $tb [, JOIN = ] @{join}
%end

%if @{hashes} %then
 $br $tb [, HASHES ]
%end

%if @{merges} %then
 $br $tb [, MERGES ]
%end

%if @{sort-op} %then
 $br $tb [, SORT1 = ] @{sort-op}
%end

%if @{sort2-op} %then
 $br $tb [, SORT2 = ] @{sort2-op}
%end

%if @{less-op} %then
 $br $tb [, LTCMP = ] @{less-op}
%end

%if @{greater-op} %then
 $br $tb [, GTCMP = ] @{greater-op}
%end

); $br

%if @{owner} %then @{owner} %end
%if @{comment} %then @{comment} %end
