# Basic structure for the diff script file generated by diff process
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {has-changes} %then
	%set {ddl-end} $br [-- ddl-end --] $br

	[-- ** Diff code generated with pgModeler (PostgreSQL Database Modeler)] $br
	[-- ** pgModeler version: ] {pgmodeler-ver} $br
	[-- ** Diff date: ] {date} $br
	[-- ** Source model: ] {dbmodel} $br
	[-- ** Database: ] {database} $br
	[-- ** PostgreSQL version: ] {pgsql-ver} $br

	$br
	[-- ** ] $ob [ Diff summary ] $cb $br
	[-- ** Dropped objects: ] {drop} $br
	[-- ** Created objects: ] {create} $br
	[-- ** Changed objects: ] {change} $br

	%if {function} %then
		$br
		[SET check_function_bodies = false;]

		{ddl-end}
	%end

	%if {search-path} %then
		$br
		[SET search_path=] {search-path} ;
		{ddl-end}
	%end

	%if {unset-perms} %then
		$br $br
		[-- ** ] $ob [ Undone permissions ] $cb $br $br
		{unset-perms}
	%end

	%if {drop-cmds} %then
		$br $br
		[-- ** ] $ob [ Dropped objects ] $cb $br $br
		{drop-cmds}
	%end

	%if {create-cmds} %then
		$br $br
		[-- ** ] $ob [ Created objects ] $cb $br $br
		{create-cmds}
	%end

	%if {alter-cmds} %then
		$br $br
		[-- ** ] $ob [ Changed objects ] $cb $br $br
		{alter-cmds}
	%end

	%if {constr-defs} %then
		$br $br
		[-- ** ] $ob [ Created constraints ] $cb $br $br
		{constr-defs}
	%end

	%if {fk-defs} %then
		$br $br
		[-- ** ] $ob [ Created foreign keys ] $cb $br $br
		{fk-defs}
	%end

	%if {set-perms} %then
		$br $br
		[-- ** ] $ob [ Created permissions ] $cb $br $br
		{set-perms}
	%end
%end
