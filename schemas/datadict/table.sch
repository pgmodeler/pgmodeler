# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made. 

%if ({type-class} == "table") %then
	%set {colspan} 8
%else
	%set {colspan} 5
%end


<table $sp id="{schema}.{name}" $sp class="{type-class}">
$br [	] <caption [ class="tab-name"]>
$br [		] <em>{schema}</em>.<strong>{name}</strong>
$br [		] <span $sp class="type-label">{type}</span>
$br [	] </caption>
$br [	] <thead>

%if {comment} %then
    $br [	] <tr>
    $br [		] <td $sp colspan="{colspan}" $sp class="tab-description">
					{comment}
    $br [		] </td>
    $br [	] </tr>
%end

%if {columns} %then
	$br [		] <tr>
	$br [			] <th>Name</th>
	$br [			] <th>[Data type]</th>
	
	%if ({type-class} == "table") %then
		$br [			] <th>PK</th>
		$br [			] <th>FK</th>
		$br [			] <th>UQ</th>
	%end
	
	$br [			] <th>[Not null]</th>
	$br [			] <th>[Default value]</th>
	$br [			] <th>Description</th>
	$br [		] </tr>
%else
	$br [		] <tr>
	$br [			] <th $sp colspan="{colspan}">
	$br [				] <em>[No columns]</em>
	$br [			] </th>
	$br [		] </tr>
%end

$br [	] </thead>
$br [	] <tbody>

%if {columns} %then
	{columns}
%end

%if {constraints} %then
	$br [	] <tr>
	$br [		] <td $sp colspan="{colspan}" $sp class="nested-tab-parent">
	$br [			] <table $sp class="nested-tab">
	$br [				] <tr>
	$br [					] <td [ class="title" colspan="6"]>Constraints</td>
	$br [				] </tr>
	$br [				] <tr>
	$br [					] <td [ class="title"]>Name</td>
	$br [					] <td [ class="title"]>Type</td>
	$br [					] <td [ class="title"]>Column(s)</td>
	$br [					] <td [ class="title"]>References</td>
	$br [					] <td [ class="title"]>Expression</td>
	$br [					] <td [ class="title"]>Description</td>
	$br [				] </tr>

	{constraints}

	$br [			] </table>
	$br [		] </td>		
	$br [	] </tr>
%end

$br [	] </tbody>


%if {inherit} %or {partitioned-table} %or {partition-tables} %then
	$br [	] <tfoot>
	$br [		] <tr>
	$br [			] <td $sp colspan="{colspan}" $sp class="nested-tab-parent">
	$br [				] <table $sp class="nested-tab">

	%if {inherit} %then
		$br [					] <tr>
		$br [						] <td $sp class="label">Inherits:</td>
		$br [						] <td> {inherit} </td>
		$br [					] </tr>
	%end

	%if {partitioned-table} %then
		$br [					] <tr>
		$br [						] <td $sp class="label">[Partition of:]</td>
		$br [						] <td>{partitioned-table}</td>
		$br [					] </tr>
	%end
	
	%if {partition-tables} %then
		$br [					] <tr>
		$br [						] <td $sp class="label">[Partitions:]</td>
		$br [						] <td>{partition-tables}</td>
		$br [					] </tr>
	%end

	$br [				] </table>
	$br [			] </td>
	$br [		] </tr>
	$br [	] </tfoot> $br
%end

$br </table> $br


%if {index}	%then
	$br <div>

    %if {split} %and {previous} %then
		$br [	] <a $sp href="{previous}.html" $sp class="nav-link"> &larr; $sp {previous}</a>
	%end	
		
    %if {split} %then
		$br [	<a href="index.html" class="nav-link">]
	%else
		$br [	<a href="\#index" class="nav-link">]
	%end

    [&uarr; Index</a>]

    %if {split} %and {next} %then
		$br [	] <a $sp href="{next}.html" $sp class="nav-link"> {next} $sp &rarr;</a>
	%end

	$br </div> $br
%end
