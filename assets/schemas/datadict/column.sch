# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%set {spc} $br [ ]

{spc} <tr>
{spc} <td>{name}</td>
{spc} <td $sp class="data-type">{type}</td>

%if ({parent} != "view") %then
	%if ({parent} == "table") %then
		{spc} <td $sp class="bool-field">{pk-constr}</td>
		{spc} <td $sp class="bool-field">{fk-constr}</td>
		{spc} <td $sp class="bool-field">{uq-constr}</td>
	%end

	{spc} <td $sp class="bool-field">{not-null}</td>
	{spc} <td $sp class="value">{default-value}</td>
	{spc} <td><p $sp class="max-td-wth"><em>{comment}</em></p></td>
%end

{spc} </tr>
