# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%set {spc} \n \s

{spc} <tr>
{spc} <td> {name} </td>
{spc} <td [ class="value constr-type"] > {type} </td>
{spc} <td> {columns} </td>
{spc} <td>

%if {ref-table} %then
	%if {split} %then
		<a \s href="{ref-table}.html"> {ref-table} </a>
	%else
		<a \s href="\# {ref-table}"> {ref-table} </a>
	%end
%end

</td>

%if ({type} != "FOREIGN KEY") %then
	{spc} <td> \&nbsp; </td>
	{spc} <td> \&nbsp; </td>
%else
	{spc} <td [ class="value"] > {upd-action} </td>
	{spc} <td [ class="value"] > {del-action} </td>
%end

{spc} <td [ class="value"] >

%if ({type} == "CHECK") %then
	<p \s class="max-td-wth"> {expression} </p>
%end

</td>

{spc} <td [ colspan="3" ] > <p \s class="max-td-wth"><em> {comment} </em></p></td>
{spc} </tr>
