# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%set {spc} \n \s

{spc} <tr>
{spc} <td> {name} </td>
{spc} <td [ class="value constr-type"] > {type} </td>
{spc} <td> {columns} </td>
{spc} <td [ class="value"] > <p \s class="max-td-wth"> {expressions} </p> </td>
{spc} <td [ class="value"] > <p \s class="max-td-wth"> {predicate} </p> </td>
{spc} <td> <p \s class="max-td-wth"> <em> {comment} </em> </p></td>
</td>

{spc} </tr>
