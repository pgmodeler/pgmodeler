# SQL definition for database model
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[-- Database generated with pgModeler (PostgreSQL Database Modeler).] $br
[-- pgModeler  version: ] {pgmodeler-ver} $br
[-- PostgreSQL version: ] {pgsql-ver} $br
[-- Project Site: pgmodeler.io] $br
[-- Model Author: ]

# This is a special token that pgModeler recognizes as end of DDL command
# when exporting models directly to DBMS. DO NOT REMOVE THIS TOKEN!
%set {ddl-end} $br [-- ddl-end --] $br

%if {author} %then
 {author} 
%else
  ---
%end

$br

%if {export-to-file} %then

 %if {role} %then {role} %end
 
 %if {tablespace} %then 
   [-- Tablespaces creation must be performed outside a multi lined SQL file. ] $br
   [-- These commands were put in this file only as a convenience.] $br
   [-- ] $br
   {tablespace} $br
 %end

 $br
 
 [-- Database creation must be performed outside a multi lined SQL file. ] $br
 [-- These commands were put in this file only as a convenience.] $br
 [-- ] $br
 {database} $br
%end

%if {function} %then
 [SET check_function_bodies = false;] 
 {ddl-end} $br
%end

%if {schema} %then
 {schema}
 [SET search_path TO ] {search-path}; 
 {ddl-end} $br
%end

%if {shell-types} %then {shell-types} %end
%if {objects} %then {objects} %end
%if {permission} %then {permission} %end

$br
