# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%set {spc} \n \s

{spc} <tr>
{spc} <td> {name} </td>
{spc} <td [ class="value bool-value"] > {cycle} </td>
{spc} <td [ class="value"] > {min-value} </td>
{spc} <td [ class="value"] > {max-value} </td>
{spc} <td> {columns} </td>
{spc} <td> <p \s class="max-td-wth"> <em> {comment} </em> </p></td>

{spc} </tr>
