# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made. 
 
$br [	] <tr>
$br [		] <td>{name}</td>
$br [		] <td $sp class="data-type">{type}</td>

%if ({parent} != "view") %then
	
	%if ({parent} == "table") %then
		$br [		] <td $sp class="bool-field">{pk-constr}</td>
		$br [		] <td $sp class="bool-field">{fk-constr}</td>
		$br [		] <td $sp class="bool-field">{uq-constr}</td>
	%end
		
	$br [		] <td $sp class="bool-field">{not-null}</td>
	$br [		] <td $sp class="value">{default-value}</td>
	$br [		] <td><p $sp class="max-td-wth"><em>{comment}</em></p></td>
%end

$br [	] </tr>
