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
    $br [		] <td [ colspan="2" class="tab-description"]>
					{comment}
    $br [		] </td>
    $br [	] </tr>
%end

%if {columns} %then
	$br [		] <tr>
	$br [			] <th>Name</th>
	$br [			] <th>[Data type]</th>
	$br [		] </tr>
%else
	$br [		] <tr>
	$br [			] <th [ colspan="2"]>
	$br [				] <em>[No columns]</em>
	$br [			] </th>
	$br [		] </tr>
%end

$br [	] </thead>
$br [	] <tbody>

%if {columns} %then
	{columns}
%end

$br [	] </tbody>


%if {references}  %then
	$br [	] <tfoot>
	$br [		] <tr>
	$br [			] <td $sp colspan="2" $sp class="nested-tab-parent">
	$br [				] <table $sp class="nested-tab">
	$br [					] <tr>
	$br [						] <td $sp class="label">References:</td>
	$br [						] <td> {references} </td>
	$br [					] </tr>
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
