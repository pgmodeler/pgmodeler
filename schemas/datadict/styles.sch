# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made. 
[body {
	font-family: sans-serif;
	color: \#333;
	margin-left: 2em;
	margin-top: 2em;
	background-color: white;
}

footer {
	margin-top: 3em;
	margin-bottom: .5em;
	font-size: 75%;
}

h3 {
	margin-bottom: .5em;
}

a {
	color: \#4182c3;
	text-decoration: none;
}

a:hover {
	color: \#080;
	text-decoration: underline;
}

\#index {
	background-color: \#f8f9fa;
	margin-block-end: 1.5em;
	list-style: none;
	display: inline-block;
	padding: 1em;
	border: 1px solid \#c0c0c0;
	border-radius: 4px 4px 4px 4px;
}

\#index li {
	list-style: none;
	margin: 0;
}

\#index > li > ul {
	padding-inline-start: 1em;
}

.table, .foreigntable, .view {
	font-size: 11pt;
	border-collapse: collapse;
	margin-top: 3em;
	margin-bottom: 1em;
}

.table caption {
	background-color: \#d2f3ff;
}

.foreigntable caption {	
	background-color: \#94f0b1;
}

.view caption {	
	background-color: \#ffd8a1;
}

.table,
.table caption, 
.table th, 
.table td {
	border: 1px solid \#b4d0da;
}

.foreigntable, 
.foreigntable caption, 
.foreigntable th, 
.foreigntable td {
	border: 1px solid \#6aad80;
}

.view, 
.view caption, 
.view th, 
.view td {
	border: 1px solid \#ba7c00;
}

.table caption, 
.foreigntable caption, 
.view caption {
	border-bottom: 0;
}

.table th,
.table .title,
.table .label{
	background-color: \#e9f8ff;
}

.foreigntable th,
.foreigntable .title,
.foreigntable .label {
	background-color: \#e2ffed;
}

.view th,
.view .title,
.view .label {
	background-color: \#ffebd2;
}

.type-label {
	float: right;
	color: \#212529;
	font-size: 70%;
	font-weight: bold;
	background-color: \#f8f9fa;
	padding: 5px;
	border-radius: 4px 4px 4px 4px;
}

caption {
	border-radius: 5px 5px 0 0;	
}

.nav-link {
	font-size: 85%;
	background-color: \#f2f2f3;
	padding: 5px;
	border-radius: 4px 4px 4px 4px;
	color: \#212529;
	font-weight: bold;
	text-decoration: none;
	margin-right: .5em;
}

.nav-link:hover {
	color: white;
	background-color: \#007bff;
}

td, th, caption, .title {
	margin: 0;
	padding: .3em;
}

.title, th {
	font-weight: bold;
	text-align: center;
	color: \#535c67;
}

.data-type, .value, .constr-type {
	font-family: monospace;
	text-align: center;
}

.constr-type {
	color: \#24486c;
}

.data-type {            
	color: \#b00;
}

.bool-field {
	text-align: center;
	color: \#080;
}

.label {
	font-weight: bold;
	color: \#444f53;
	width: 1px;
	white-space: nowrap;
}

.tab-name {
	font-size: 112%;
}

.tab-description {
	font-style: italic;
	padding: .5em;
}

.nested-tab-parent {
	padding: 0;
}

.nested-tab {
	font-size: 11pt;
	width: 100%;
	border-collapse: collapse;
}

.nested-tab > tbody > tr,
.nested-tab > tr{
	padding: 0;
}

.nested-tab td {
	border-bottom: 0;
	border-left: 0;
}

.nested-tab > tbody > tr:first-child td,
.nested-tab > tr:first-child td {
	border-top: 0;
}

.nested-tab td:last-child {
	border-right: 0;
}

.max-td-wth {
    max-width: 500px;
}

]
