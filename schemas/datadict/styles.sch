# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made. 
[body {
	font-family: sans-serif;
	color: \#333;
}

.table, .foreigntable, .view {
	font-size: 11pt;
	border-collapse: collapse;
	margin-top: 1.2em;
	margin-bottom: 1.2em;
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
	background-color: \#d2f3ff;
}

.table, caption, th, td {
	border: 1px solid \#ccc;
}

caption {
	border-bottom: none;
}

th, .title, .nav-link {
	background-color: \#e9f8ff;
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
	margin-bottom: 1em;
}

.nav-link:hover {
	color: white;
	background-color: \#007bff;
}

td, th, caption, .title {
	margin: 0;
	padding: .3em;
}

.title {
	font-weight: bold;
}

.data-type, .value {
	font-family: monospace;
	text-align: center;
}

.data-type {            
	color: \#c00;
}

.bool-field {
	text-align: center;
	color: \#080;
}

.label {
	font-weight: bold;
	font-style: italic;
}

.tab-name {
	font-size: 110%;
}

.tab-description {
	font-style: italic;
	padding: .5em;
}

\#index {
	margin-block-end: 5em;
}

]
