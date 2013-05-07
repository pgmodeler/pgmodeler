# SQL definition for database model
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- Database generated with pgModeler (PostgreSQL Database Modeler).] $br

[-- PostgreSQL version: ]
%if @{pgsql90} %then @{pgsql90} %end
%if @{pgsql91} %then @{pgsql91} %end
%if @{pgsql92} %then @{pgsql92} %end
$br

[-- Project Site: pgmodeler.com.br] $br
[-- Model Author: ]

%if @{author} %then
 @{author} 
%else
  ---
%end
$br $br

[SET check_function_bodies = false;] $br
[-- ddl-end --] $br $br

%if @{export-to-file} %then

 %if @{role} %then @{role} %end
 %if @{tablespace} %then 
   [-- Tablespaces creation must be done outside an multicommand file.] $br
   [-- These commands were put in this file only for convenience.] $br
   @{tablespace} $br
 %end

$br
    [-- Database creation must be done outside an multicommand file.] $br
    [-- These commands were put in this file only for convenience.] $br
   @{database} $br
%end

%if @{schema} %then @{schema} %end
%if @{shell-types} %then @{shell-types} %end
%if @{objects} %then @{objects} %end
%if @{permission} %then @{permission} %end

$br
