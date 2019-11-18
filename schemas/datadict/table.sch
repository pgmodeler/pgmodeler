# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made. 

%set {brk} $br $tb
%set {brk-tb} $br $tb $tb

<table>
{brk} <caption>
{brk-tb} <em>{schema}</em>.<strong>{name}</strong>
{brk} </caption>
{brk} <thead>
{brk-tb} <tr>
{brk-tb} $tb <th>Name</th>
{brk-tb} $tb <th>[Data type]</th>
{brk-tb} $tb <th>PK</th>
{brk-tb} $tb <th>FK</th>
{brk-tb} $tb <th>UQ</th>
{brk-tb} $tb <th>[Not null]</th>
{brk-tb} $tb <th>[Default value]</th>
{brk-tb} $tb <th>Comment</th>
{brk-tb} </tr>
{brk} </thead>
{brk} <tbody>
{rows}
{brk} </tbody>
{brk} <tfoot>
{brk-tb} <tr>
{brk-tb} $tb <td $sp class="label">Comment:</td>
{brk-tb} $tb <td $sp colspan="7">{comment}</td>
{brk-tb} </tr>
{brk-tb} <tr>
{brk-tb} $tb <td $sp class="label">[Inherits: ]</td>
{brk-tb} $tb <td $sp colspan="7">{inherit}</td>
{brk-tb} </tr> $br
{brk-tb} <tr>
{brk-tb} $tb <td $sp class="label">[Partition of: ]</td>
{brk-tb} $tb <td $sp colspan="7">{partitioned-table}</td>
{brk-tb} </tr> $br
{brk} </tfoot> $br
</table> $br
