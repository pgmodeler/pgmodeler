# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {datadictidx} %then
	\n <div>

	%if {split} %and {previous} %then
		{spc} <a \s href="{previous}.html" \s class="nav-link"> [&larr;] \s {previous} </a>
	%end

	%if {split} %then
		\n [ <a href="index.html" class="nav-link"> ]
	%else
		\n [ <a href="#index" class="nav-link"> ]
	%end

	[ &uarr; Index</a> ]

	%if {split} %and {next} %then
		{spc} <a \s href="{next}.html" \s class="nav-link"> {next} \s [&rarr;] </a>
	%end

	\n </div> \n
%end
