# SQL definition for database model
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- Database generated with pgModeler (PostgreSQL Database Modeler).] $br
[-- pgModeler  version: ] {pgmodeler-ver} $br
[-- PostgreSQL version: ] {pgsql-ver} $br
[-- Project Site: pgmodeler.io] $br
[-- Model Author: ]

%if {author} %then
 {author} 
%else
  ---
%end
$br $br

%if {function} %then
 [SET check_function_bodies = false;] $br
 [-- ddl-end --] $br $br
%end

%if {export-to-file} %then

 %if {role} %then {role} %end
 %if {tablespace} %then 
   [-- Tablespaces creation must be done outside a multicommand file.] $br
   [-- These commands were put in this file only as a convenience.] $br
   {tablespace} $br
 %end

$br
    [-- Database creation must be done outside a multicommand file.] $br
    [-- These commands were put in this file only as a convenience.] $br
   {database} $br
%end

%if {schema} %then
 {schema}
 [SET search_path TO ] {search-path}; $br
 [-- ddl-end --] $br $br
%end

%if {shell-types} %then {shell-types} %end
%if {objects} %then {objects} %end
%if {permission} %then {permission} %end

$br
