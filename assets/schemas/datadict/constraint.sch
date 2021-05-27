# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made. 
 
$br [				] <tr>
$br [					] <td>{name}</td>
$br [					] <td [ class="value constr-type"]>{type}</td>
$br [					] <td>{columns}</td>
$br [					] <td>

%if {ref-table} %then
    %if {split} %then
		<a $sp href="{ref-table}.html">{ref-table}</a>
	%else
		<a $sp href="\#{ref-table}">{ref-table}</a>
	%end
%end

</td>

$br [					] <td [ class="value"]>
    %if ({type} == "CHECK") %then
	    <p $sp class="max-td-wth">{expression}</p>
	%end
</td>

$br [					] <td [ colspan="3"]><p $sp class="max-td-wth"><em>{comment}</em></p></td>
$br [				] </tr>
