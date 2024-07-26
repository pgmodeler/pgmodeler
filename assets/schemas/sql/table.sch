# SQL definition for tables
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "ddlend"
@include "objlabel"
@include "prependedsql"

[CREATE]

%if {unlogged} %then
	[ UNLOGGED]
%end

[ TABLE ] {name}

%if {partitioned-table} %then
	$br [PARTITION OF ] {partitioned-table} $sp
%end

%if %not {partitioned-table} %then

	[ (] $br

	%if {copy-table} %then
		$tb LIKE $sp {copy-table}

		%if %not {gen-alter-cmds} %then
			%if {columns} %or {constraints} %then
				[,]
			%end
		%end

		$br
	%end

	%if %not {gen-alter-cmds} %then
		%if {columns} %then
			{columns}
		%end

		%if {inh-columns} %then
			{inh-columns}
		%end

		%if {constraints} %then
			{constraints}
		%end
	%end


	%if %not {constraints} %then
		$br
	%end

	)
%else
	%if %not {gen-alter-cmds} %and {partitioned-table} %and {constraints} %then
		[ (] $br {constraints} $br [)]
	%end
%end

%if {partitioned-table} %then
	%if {partition-bound-expr} %then
		$br [FOR VALUES ] {partition-bound-expr}
	%else
		DEFAULT
	%end
%end

%if {partitioning} %then
	$br [PARTITION BY ] {partitioning} [ (] {partitionkey} [)]
%end

%if {ancestor-table} %then
	$br [ INHERITS(] {ancestor-table} [)]
%end

%if ({pgsql-ver} <=f "11.0") %and {oids} %then
	$br [WITH ( OIDS = TRUE )]
%end

%if {tablespace} %then
	$br [TABLESPACE ] {tablespace}
%end

;

{ddl-end}

%if {gen-alter-cmds} %then
	%if {columns} %then
		$br {columns}
	%end

	%if {constraints} %then
		$br {constraints}
	%end
%end

%if {comment} %then
	{comment}
%end

%if {cols-comment} %then
	{cols-comment}
%end

%if {owner} %then
	{owner}
%end

%if {rls-enabled} %then
	[ALTER TABLE ] {name} [ ENABLE ROW LEVEL SECURITY;]

	{ddl-end}

	%if {rls-forced} %then
		[ALTER TABLE ] {name} [ FORCE ROW LEVEL SECURITY;]

		{ddl-end}
	%end
%end

%if {appended-sql} %then
	{appended-sql}
	{ddl-end}
%end

%if {initial-data} %then
	$br {initial-data} $br
%end

$br
