# Template code for data dictionary generation
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%set {spc} \n\#\#\#\#\s
%set {colsdiv} [ --- | --- | --- |]
%set {t6cols} [| ] {colsdiv} {colsdiv} \n

%if {columns} %then
 	{columns}
%end

%if {constraints} %then
	{spc} Constraints \n

	[| Name | Type | Column(s) | References | Expression | Description |] \n
	{t6cols}

	{constraints}
%end

%if {indexes} %then
	{spc} Indexes \n

	[| Name | Type | Column(s) | Expression(s) | Predicate | Description |] \n
	{t6cols}

	{indexes}
%end

%if {triggers} %then
 	{spc} Triggers \n

 	[| Name | Attributes | Function | Firing mode | On event(s) | Per row | Condition | References | Description |] \n
 	[| ] {colsdiv} [ --- | --- | :---: |] {colsdiv} \n

  	{triggers}
%end

%if {sequences} %then
 	{spc} Sequences \n

 	[| Name | Cyclic | Min. value | Max. value | Column(s) | Description |] \n
 	[| --- | :---: | --- |] {colsdiv} \n

  	{sequences}
%end