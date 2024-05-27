# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

@include "header"

%set {spc} \n \s

\n [ <h2>Data dictionary index</h2> ]

{spc} <ul \s id="index">
{spc} <li><strong> [Database: ] </strong> {name} </strong>

%if {table} %then
	{spc} <li><br/><strong> Tables </strong>
	{spc} <ul>
	{table}
	{spc} </ul>
	{spc} </li>
%end

%if {foreigntable} %then
	{spc} <li><br/><strong> [Foreign tables] </strong>
	{spc} <ul>
	{foreigntable}
	{spc} </ul>
	{spc} </li>
%end

%if {view} %then
	{spc} <li><br/><strong> Views </strong>
	{spc} <ul>
	{view}
	{spc} </ul>
	{spc} </li>
%end

{spc} </ul>
\n

%if {split} %then
	@include "footer"
%end
