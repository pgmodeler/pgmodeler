# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%set {spc} \n \s

%if ({type-class} == "table") %then
	%set {colspan} 8
%else
	%set {colspan} 5
%end

%if {columns} %then
	{columns}
%end

%if {constraints} %then
	{spc} <tr>
	{spc} <td \s colspan="{colspan}" \s class="nested-tab-parent">
	{spc} <table \s class="nested-tab">
	{spc} <tr>
	{spc} <td [ class="title" colspan="8"] > Constraints </td>
	{spc} </tr>
	{spc} <tr>
	{spc} <td [ class="title"] > Name </td>
	{spc} <td [ class="title"] > Type </td>
	{spc} <td [ class="title"] > Column(s) </td>
	{spc} <td [ class="title"] > References </td>
	{spc} <td [ class="title"] > On \s Update </td>
	{spc} <td [ class="title"] > On \s Delete </td>
	{spc} <td [ class="title"] > Expression </td>
	{spc} <td [ class="title"] > Description </td>
	{spc} </tr>

	{constraints}

	{spc} </table>
	{spc} </td>
	{spc} </tr>
%end

%if {indexes} %then
	{spc} <tr>
	{spc} <td \s colspan="{colspan}" \s class="nested-tab-parent">
	{spc} <table \s class="nested-tab">
	{spc} <tr>
	{spc} <td [ class="title" colspan="6"] > Indexes </td>
	{spc} </tr>
	{spc} <tr>
	{spc} <td [ class="title"] > Name </td>
	{spc} <td [ class="title"] > Type </td>
	{spc} <td [ class="title"] > Column(s) </td>
	{spc} <td [ class="title"] > Expression(s) </td>
	{spc} <td [ class="title"] > Predicate </td>
	{spc} <td [ class="title"] > Description </td>
	{spc} </tr>

	{indexes}

	{spc} </table>
	{spc} </td>
	{spc} </tr>
%end

%if {triggers} %then
	{spc} <tr>
	{spc} <td \s colspan="{colspan}" \s class="nested-tab-parent">
	{spc} <table \s class="nested-tab">
	{spc} <tr>
	{spc} <td [ class="title" colspan="9"] > Triggers </td>
	{spc} </tr>
	{spc} <tr>
	{spc} <td [ class="title"] > Name </td>
	{spc} <td [ class="title"] > Attributes </td>
	{spc} <td [ class="title"] > Function </td>
	{spc} <td [ class="title"] > [Firing mode] </td>
	{spc} <td [ class="title"] > [On event(s)] </td>
	{spc} <td [ class="title"] > [Per row] </td>
	{spc} <td [ class="title"] > Condition </td>
	{spc} <td [ class="title"] > References </td>
	{spc} <td [ class="title"] > Description </td>
	{spc} </tr>

	{triggers}

	{spc} </table>
	{spc} </td>
	{spc} </tr>
%end

%if {sequences} %then
	{spc} <tr>
	{spc} <td \s colspan="{colspan}" \s class="nested-tab-parent">
	{spc} <table \s class="nested-tab">
	{spc} <tr>
	{spc} <td [ class="title" colspan="6"] > Sequences </td>
	{spc} </tr>
	{spc} <tr>
	{spc} <td [ class="title"] > Name </td>
	{spc} <td [ class="title"] > Cyclic </td>
	{spc} <td [ class="title"] > [Min. value] </td>
	{spc} <td [ class="title"] > [Max. value] </td>
	{spc} <td [ class="title"] > Column(s) </td>
	{spc} <td [ class="title"] > Description </td>
	{spc} </tr>

	{sequences}

	{spc} </table>
	{spc} </td>
	{spc} </tr>
%end