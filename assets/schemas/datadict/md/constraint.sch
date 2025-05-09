# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

[| ] {name} [ | ] {type} [ | ] {columns} [ | ]

%if {ref-table} %then
	%if {split} %then
		\[ {ref-table} \] ({ref-table}.md)
	%else
		\[ {ref-table} \] (\# {ref-table})
	%end
%end

[ | ]

%if ({type} != "FOREIGN KEY") %then
	[ | ]
%else
	{upd-action} [ | ] {del-action}
%end

[ | ]

%if ({type} == "CHECK") %then
	{expression}
%end

[ | ] {comment} [ |] \n
