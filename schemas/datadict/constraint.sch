# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made. 
 
$br [				] <tr>
$br [					] <td>{name}</td>
$br [					] <td [ class="value constr-type"]>{type}</td>
$br [					] <td>{columns}</td>
$br [					] <td>

%if {ref-table} %then
	%if {splitted} %then
		<a $sp href="{ref-table}.html">{ref-table}</a>
	%else
		<a $sp href="\#{ref-table}">{ref-table}</a>
	%end
%end

</td>

$br [					] <td [ colspan="4"]><em>{comment}</em></td>
$br [				] </tr>
