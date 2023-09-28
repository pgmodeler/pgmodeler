# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if ({type-class} == "table") %then
	%set {colspan} 8
%else
	%set {colspan} 5
%end

%set {spc} $br [ ]


<table $sp id="{schema}.{name}" $sp class="{type-class}">
{spc} <caption [ class="tab-name"]>
{spc} <em>{schema}</em>.<strong>{name}</strong>
{spc} <span $sp class="type-label">{type}</span>
{spc} </caption>
{spc} <thead>

%if {comment} %then
	{spc} <tr>
	{spc} <td $sp colspan="{colspan}" $sp class="tab-description">
	{comment}
	{spc} </td>
	{spc} </tr>
%end

%if {columns} %then
	{spc} <tr>
	{spc} <th> Name </th>
	{spc} <th>[Data type]</th>

	%if ({type-class} == "table") %then
		{spc} <th>PK</th>
		{spc} <th>FK</th>
		{spc} <th>UQ</th>
	%end

	{spc} <th>[Not null]</th>
	{spc} <th>[Default value]</th>
	{spc} <th> Description </th>
	{spc} </tr>
%else
	{spc} <tr>
	{spc} <th $sp colspan="{colspan}">
	{spc} <em>[No columns]</em>
	{spc} </th>
	{spc} </tr>
%end

{spc} </thead>
{spc} <tbody> 

%if {objects} %then
	{objects}
%end

{spc} </tbody>

%if {inherit} %or {partitioned-table} %or {partition-tables} %then
	{spc} <tfoot>
	{spc} <tr>
	{spc} <td $sp colspan="{colspan}" $sp class="nested-tab-parent">
	{spc} <table $sp class="nested-tab">

	%if {inherit} %then
		{spc} <tr>
		{spc} <td $sp class="label"> Inherits: </td>
		{spc} <td> {inherit} </td>
		{spc} </tr>
	%end

	%if {partitioned-table} %then
		{spc} <tr>
		{spc} <td $sp class="label">[Partition of:]</td>
		{spc} <td> {partitioned-table} </td>
		{spc} </tr>
	%end

	%if {partition-tables} %then
		{spc} <tr>
		{spc} <td $sp class="label">[Partitions:]</td>
		{spc} <td> {partition-tables} </td>
		{spc} </tr>
	%end

	{spc} </table>
	{spc} </td>
	{spc} </tr>
	{spc} </tfoot> $br
%end

$br </table> $br


%if {datadictidx} %then
	$br <div>

	%if {split} %and {previous} %then
		{spc} <a $sp href="{previous}.html" $sp class="nav-link"> [&larr;] $sp {previous}</a>
	%end

	%if {split} %then
		$br [ <a href="index.html" class="nav-link">]
	%else
		$br [ <a href="\#index" class="nav-link">]
	%end

	[&uarr; Index</a>]

	%if {split} %and {next} %then
		{spc} <a $sp href="{next}.html" $sp class="nav-link"> {next} $sp [&rarr;] </a>
	%end

	$br </div> $br
%end
