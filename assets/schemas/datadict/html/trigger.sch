# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%set {spc} \n \s

{spc} <tr>
{spc} <td> {name} </td>
{spc} <td [ class="value constr-type"] > {attributes} </td>
{spc} <td> {function} </td>
{spc} <td [ class="value constr-type"] > {firing-type} </td>
{spc} <td [ class="value constr-type"] > {events} </td>
{spc} <td [ class="bool-field"] > {per-line} </td>
{spc} <td [ class="value"] > <p \s class="max-td-wth"> {condition} </p> </td>

{spc} <td>
	%if {ref-table} %then
		%if {split} %then
			<a \s href="{ref-table}.html"> {ref-table} </a>
		%else
			<a \s href="\# {ref-table}"> {ref-table} </a>
		%end
	%end
{spc} </td>

{spc} <td> <p \s class="max-td-wth"> <em> {comment} </em> </p></td>
</td>

{spc} </tr>
