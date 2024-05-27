# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%set {spc} \n \s

<table \s id="{schema}.{name}" \s class="{type-class}">
{spc} <caption [ class="tab-name"] >
{spc} <em> {schema} </em> . <strong> {name} </strong>
{spc} <span \s class="type-label"> {type} </span>
{spc} </caption>
{spc} <thead>

%if {comment} %then
	{spc} <tr>
	{spc} <td [ colspan="2" class="tab-description"] >
	{comment}
	{spc} </td>
	{spc} </tr>
%end

%if {columns} %then
	{spc} <tr>
	{spc} <th> Name </th>
	{spc} <th> [Data type] </th>
	{spc} </tr>
%else
	{spc} <tr>
	{spc} <th [ colspan="2"] >
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


%if {references} %then
	{spc} <tfoot>
	{spc} <tr>
	{spc} <td \s colspan="2" \s class="nested-tab-parent">
	{spc} <table \s class="nested-tab">
	{spc} <tr>
	{spc} <td \s class="label">References:</td>
	{spc} <td> {references} </td>
	{spc} </tr>
	{spc} </table>
	{spc} </td>
	{spc} </tr>
	{spc} </tfoot> \n
%end

\n </table> \n

@include "navigation"
