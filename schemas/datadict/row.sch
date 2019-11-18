# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made. 
 
%set {brk} $br $tb
%set {brk-tb} $br $tb $tb

{brk} <tr>
{brk-tb} <td>{name}</td>
{brk-tb} <td $sp class="data-type">{type}</td>
{brk-tb} <td $sp class="bool-field">{pk-constr}</td>
{brk-tb} <td $sp class="bool-field">{fk-constr}</td>
{brk-tb} <td $sp class="bool-field">{uq-constr}</td>
{brk-tb} <td $sp class="bool-field">{not-null}</td>
{brk-tb} <td>{default-value}</td>
{brk-tb} <td>{comment}</td>
{brk} </tr>
