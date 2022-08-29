# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%set {spc} $br [ ]

{spc} <tbody> 

%if {columns} %then
	{columns}
%end

%if {constraints} %then
	{spc} <tr>
	{spc} <td $sp colspan="{colspan}" $sp class="nested-tab-parent">
	{spc} <table $sp class="nested-tab">
	{spc} <tr>
	{spc} <td [ class="title" colspan="6"]> Constraints </td>
	{spc} </tr>
	{spc} <tr>
	{spc} <td [ class="title"]> Name </td>
	{spc} <td [ class="title"]> Type </td>
	{spc} <td [ class="title"]> Column(s) </td>
	{spc} <td [ class="title"]> References </td>
	{spc} <td [ class="title"]> Expression </td>
	{spc} <td [ class="title"]> Description </td>
	{spc} </tr>

	{constraints}

	{spc} </table>
	{spc} </td>
	{spc} </tr>
%end

%if {indexes} %then
	{spc} <tr>
	{spc} <td $sp colspan="{colspan}" $sp class="nested-tab-parent">
	{spc} <table $sp class="nested-tab">
	{spc} <tr>
	{spc} <td [ class="title" colspan="6"]> Indexes </td>
	{spc} </tr>
	{spc} <tr>
	{spc} <td [ class="title"]> Name </td>
	{spc} <td [ class="title"]> Type </td>
	{spc} <td [ class="title"]> Column(s) </td>
	{spc} <td [ class="title"]> Expression(s) </td>
	{spc} <td [ class="title"]> Predicate </td>
	{spc} <td [ class="title"]> Description </td>
	{spc} </tr>

	{indexes}

	{spc} </table>
	{spc} </td>
	{spc} </tr>
%end


{spc} </tbody>