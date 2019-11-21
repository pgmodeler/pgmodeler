# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made. 

<table $sp id="{schema}.{name}" $sp class="{type-class}">
$br [	] <caption [ class="tab-name"]>
$br [		] <em>{schema}</em>.<strong>{name}</strong>
$br [		] <span $sp class="type-label">{type}</span>
$br [	] </caption>
$br [	] <thead>

%if {comment} %then
    $br [	] <tr>
    $br [		] <td [ colspan="8" class="tab-description"]>
					{comment}
    $br [		] </td>
    $br [	] </tr>
%end

%if {columns} %then
	$br [		] <tr>
	$br [			] <th>Name</th>
	$br [			] <th>[Data type]</th>
	$br [			] <th>PK</th>
	$br [			] <th>FK</th>
	$br [			] <th>UQ</th>
	$br [			] <th>[Not null]</th>
	$br [			] <th>[Default value]</th>
	$br [			] <th>Description</th>
	$br [		] </tr>
%else
	$br [		] <tr>
	$br [			] <th [ colspan="8"]>
	$br [				] <em>[No columns]</em>
	$br [			] </th>
	$br [		] </tr>
%end

$br [	] </thead>
$br [	] <tbody>

%if {columns} %then
	{columns}
%end

$br [		] <tr>
$br [			] <td [ class="title" colspan="8"]>			
$br [				]

%if {constraints} %then
 Constraints
%else
 <em>[No constraints]</em> 
%end

$br [			] </td>
$br [		] </tr>

%if {constraints} %then
	$br [		] <tr>
	$br [			] <td [ class="title"]>Name</td>
	$br [			] <td [ class="title"]>Type</td>
	$br [			] <td [ class="title"]>Column(s)</td>
	$br [			] <td [ class="title"]>References</td>
	$br [			] <td [ class="title" colspan="4"]>Description</td>
	$br [		] </tr>

	{constraints}
%end


$br [	] </tbody>
$br [	] <tfoot>
$br [		] <tr>
$br [			] <td [ colspan="8">]
$br [				] <table [ width="100%">]
$br [					] <tr>
$br [						] <td $sp class="label">[Inherits: ]</td>
$br [						] <td>{inherit}</td>
$br [					] </tr>
$br [					] <tr>
$br [						] <td $sp class="label">[Partition of: ]</td>
$br [						] <td>{partitioned-table}</td>
$br [					] </tr>
$br [				] </table>
$br [			] </td>
$br [		] </tr>
$br [	] </tfoot> $br
</table> $br

$br <div>

%if {splitted} %and {previous} %then
	$br [	] <a $sp href="{previous}.html" $sp class="nav-link"> &larr; $sp {previous}</a>
%end	
	
%if {splitted} %then
 [<a href="index.html" class="nav-link">]
%else
 [<a href="\#index" class="nav-link">]
%end

[&uarr; Index</a>]

%if {splitted} %and {next} %then
	$br [	] <a $sp href="{next}.html" $sp class="nav-link"> {next} $sp &rarr;</a>
%end

$br </div> $br

