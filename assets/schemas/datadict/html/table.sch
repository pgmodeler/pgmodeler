# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if ({type-class} == "table") %then
	%set {colspan} 8
%else
	%set {colspan} 5
%end

%set {spc} \n \s


<table \s id="{schema}.{name}" \s class="{type-class}">
{spc} <caption [ class="tab-name"] >
{spc} <em> {schema} </em> . <strong> {name} </strong>
{spc} <span \s class="type-label"> {type} </span>
{spc} </caption>
{spc} <thead>

%if {comment} %then
	{spc} <tr>
	{spc} <td \s colspan="{colspan}" \s class="tab-description">
	{comment}
	{spc} </td>
	{spc} </tr>
%end

%if {columns} %then
	{spc} <tr>
	{spc} <th> Name </th>
	{spc} <th> [Data type] </th>

	%if ({type-class} == "table") %then
		{spc} <th> PK </th>
		{spc} <th> FK </th>
		{spc} <th> UQ </th>
	%end

	{spc} <th> [Not null] </th>
	{spc} <th> [Default value] </th>
	{spc} <th> Description </th>
	{spc} </tr>
%else
	{spc} <tr>
	{spc} <th \s colspan="{colspan}">
	{spc} <em> [No columns] </em>
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
	{spc} <td \s colspan="{colspan}" \s class="nested-tab-parent">
	{spc} <table \s class="nested-tab">

	%if {inherit} %then
		{spc} <tr>
		{spc} <td \s class="label"> Inherits: </td>
		{spc} <td> {inherit} </td>
		{spc} </tr>
	%end

	%if {partitioned-table} %then
		{spc} <tr>
		{spc} <td \s class="label"> [Partition of:] </td>
		{spc} <td> {partitioned-table} </td>
		{spc} </tr>
	%end

	%if {partition-tables} %then
		{spc} <tr>
		{spc} <td \s class="label"> [Partitions:] </td>
		{spc} <td> {partition-tables} </td>
		{spc} </tr>
	%end

	{spc} </table>
	{spc} </td>
	{spc} </tr>
	{spc} </tfoot> \n
%end

\n </table> \n

@include "navigation"
