# SQL definition for databases
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless that you know what
#          you are doing.

[-- object: ] @{name} [ | type: ] @{sql-object} [ -- ] $br

%if @{comment} %then [-- ] @{comment} [ --] $br %end

[CREATE DATABASE ] @{name} $br

%if @{template} %then
  $tb [TEMPLATE = ] @{template} $br
%end

%if @{encoding} %then
  $tb [ENCODING = ] @{encoding} $br
%end

%if @{tablespace} %then
  $tb [TABLESPACE = ] @{tablespace} $br
%end

%if @{owner} %then
  $tb [OWNER = ] @{owner} $br
%end

%if @{connlimit} %then
  $tb [CONNECTION LIMIT = ] @{connlimit} $br
%end

; $br
