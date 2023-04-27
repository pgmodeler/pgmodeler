# XML definition for a single object's metadata
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%set {space} $br $tb $tb

$tb [<info object=] "&{name}" [ type=] "{type}"

%if {tag} %then
	[ tag=] "&{tag}"
%end

%if {protected} %or {sql-disabled} %then
	{space}
	%if {protected} %then [ protected="true"] %end
	%if {sql-disabled} %then [ sql-disabled="true"] %end
%end

%if {author} %or {last-position} %or {last-zoom} %then
	{space}
	%if {author} %then [ author=] "&{author}" %end
	%if {last-position} %then [ last-position=] "{last-position}" %end
	%if {last-zoom} %then [ last-zoom=] "{last-zoom}" %end
%end

%if {append-at-eod} %or {prepend-at-bod} %then
	{space}
	%if {append-at-eod} %then [ append-at-eod=] "{append-at-eod}" %end
	%if {prepend-at-bod} %then [ prepend-at-bod=] "{prepend-at-bod}" %end
%end

%if {default-schema} %or {default-owner} %then
	{space}
	%if {default-schema} %then [ default-schema=] "&{default-schema}" %end
	%if {default-owner} %then [ default-owner=] "&{default-owner}" %end
%end

%if {default-collation} %or {default-tablespace} %then
	{space}
	%if {default-collation} %then [ default-collation=] "&{default-collation}" %end
	%if {default-tablespace} %then [ default-tablespace=] "&{default-tablespace}" %end
%end

%if {src-table} %then
	{space} [ src-table=] "&{src-table}" [ src-type=] "{src-type}"
%end

%if {dst-table} %then
	{space} [ dst-table=] "&{dst-table}" [ dst-type=] "{dst-type}"
%end

%if {custom-color} %or {rect-visible} %then
	{space}
	%if {custom-color} %then [ custom-color=] "{custom-color}" %end
	%if {rect-visible} %then [ rect-visible=] "{rect-visible}" %end
%end

%if {faded-out} %or {collapse-mode} %then
	{space}
	%if {faded-out} %then [ faded-out=] "{faded-out}" %end
	%if {collapse-mode} %then [ collapse-mode=] "{collapse-mode}" %end
%end

%if {alias} %then
	{space}
	%if {alias} %then [ alias=] "&{alias}" %end
	%if {table} %then [ table=] "&{table}" %end
%end

%if {z-value} %then
	{space} [ z-value=] "{z-value}"
%end

%if {layers} %then
	{space} [ layers]="&{layers}"
%end

%if {active-layers} %then
	{space} [ active-layers]="{active-layers}"
%end

%if {layer-name-colors} %then
	{space} [ layer-name-colors]="{layer-name-colors}"
%end

%if {layer-rect-colors} %then
	{space} [ layer-rect-colors]="{layer-rect-colors}"
%end

%if {show-layer-names} %then
	{space} [ show-layer-names]="{show-layer-names}"
%end

%if {show-layer-rects} %then
	{space} [ show-layer-rects]="{show-layer-rects}"
%end

%if {position} %or {appended-sql} %or {prepended-sql} %then
	> $br
	%if {position} %then {position} %end
	%if {appended-sql} %then $tb {appended-sql} %end
	%if {prepended-sql} %then $tb {prepended-sql} %end
	$tb </info>
%else
	[/>]
%end

$br
