# XML definition for basic configurations file
# CAUTION: Do not modify this file unless you know what you are doing.
[<?xml version="1.0" encoding="UTF-8" ?>] $br
[<!--] $br
[ CAUTION: Do not modify this file directly on it's code unless you know what you are doing.] $br
[ Unexpected results may occur if the code is changed deliberately.] $br
[-->] $br

%set {spc}  $tb $tb

<general> $br
$tb [<configuration pgmodeler-ver="] {pgmodeler-ver} ["] $br 
{spc} [first-run="] {first-run} ["] $br
{spc} [op-list-size="] {op-list-size} ["] $br
{spc} [autosave-interval="] {autosave-interval} ["] $br
{spc} [paper-type="] {paper-type} ["] $br
{spc} [paper-orientation="] {paper-orientation} ["] $br
{spc} [paper-margin="] {paper-margin} ["] $br

%if {paper-custom-size} %then
	{spc} [paper-custom-size="] {paper-custom-size} ["] $br
%end

{spc} [print-grid="] %if {print-grid} %then true %else false %end ["] $br
{spc} [print-pg-num="] %if {print-pg-num} %then true %else false %end ["] $br
{spc} [hide-rel-name="] %if {hide-rel-name} %then true %else false %end ["] $br
{spc} [hide-ext-attribs="] %if {hide-ext-attribs} %then true %else false %end ["] $br
{spc} [hide-table-tags="] %if {hide-table-tags} %then true %else false %end ["] $br
{spc} [canvas-corner-move="] %if {canvas-corner-move} %then true %else false %end ["] $br
{spc} [invert-rangesel-trigger="] %if {invert-rangesel-trigger} %then true %else false %end ["] $br
{spc} [check-update="] %if {check-update} %then true %else false %end ["] $br
{spc} [check-versions="] {check-versions} ["] $br
{spc} [save-last-position="] %if {save-last-position} %then true %else false %end ["] $br
{spc} [show-main-menu="] %if {show-main-menu} %then true %else false %end ["] $br
{spc} [disable-smoothness="] %if {disable-smoothness} %then true %else false %end ["] $br
{spc} [simplified-obj-creation="] %if {simplified-obj-creation} %then true %else false %end ["] $br
{spc} [confirm-validation="] %if {confirm-validation} %then true %else false %end ["] $br
{spc} [code-completion="] %if {code-completion} %then true %else false %end ["] $br
{spc} [use-placeholders="] %if {use-placeholders} %then true %else false %end ["] $br
{spc} [show-canvas-grid="] %if {show-canvas-grid} %then true %else false %end ["] $br
{spc} [show-page-delimiters="] %if {show-page-delimiters} %then true %else false %end ["] $br
{spc} [lock-page-delim-resize="] %if {lock-page-delim-resize} %then true %else false %end ["] $br
{spc} [align-objs-to-grid="] %if {align-objs-to-grid} %then true %else false %end ["] $br
{spc} [history-max-length="] {history-max-length} ["] $br
{spc} [use-curved-lines="] %if {use-curved-lines} %then true %else false %end ["] $br
{spc} [compact-view="] %if {compact-view} %then true %else false %end ["] $br
{spc} [save-restore-geometry="] %if {save-restore-geometry} %then true %else false %end ["] $br
{spc} [low-verbosity="] %if {low-verbosity} %then true %else false %end ["] $br
{spc} [escape-comment="] %if {escape-comment} %then true %else false %end ["] $br
{spc} [hide-schema-names-of-types="] %if {hide-schema-names-of-types} %then true %else false %end ["] $br
{spc} [old-pgsql-versions="] %if {old-pgsql-versions} %then true %else false %end ["] $br
{spc} [hide-obj-shadows="] %if {hide-obj-shadows} %then true %else false %end ["] $br
{spc} [truncate-column-data="] %if {truncate-column-data} %then true %else false %end ["] $br
{spc} [column-trunc-threshold="] {column-trunc-threshold} ["] $br
{spc} [disable-inline-editor="] %if {disable-inline-editor} %then true %else false %end ["] $br
{spc} [alert-unsaved-models="] {alert-unsaved-models} ["] $br
{spc} [alert-open-sqltabs="] {alert-open-sqltabs} ["] $br
{spc} [use-def-disambiguation="] {use-def-disambiguation} ["] $br
{spc} [hide-objects-sel-info="] %if {hide-objects-sel-info} %then true %else false %end ["] $br
{spc} [hide-cur-pos-zoom-info="] %if {hide-cur-pos-zoom-info} %then true %else false %end ["] $br

%if {source-editor-app} %then
	{spc} [source-editor-app="] &{source-editor-app} ["] $br
%end

%if {source-editor-app} %and {source-editor-args} %then
	{spc} [source-editor-args="] &{source-editor-args} ["] $br
%end

%if {ui-language} %then
	{spc} [ui-language="] &{ui-language} ["] $br
%end

$tb /> $br

%if {file} %then
	$br $tb <session> $br
	$tb {file}
	$tb </session> $br
%end

%if {recent-models} %then
	$br $tb <recent-models> $br
	{recent-models}
	$tb </recent-models> $br
%end

%if {dock-widgets} %then
	$br $tb <dock-widgets> $br
	{dock-widgets}
	$tb </dock-widgets> $br
%end

%if {widgets-geometry} %then
	$br $tb <widgets-geometry> $br
	{widgets-geometry}
	$tb </widgets-geometry> $br
%end

</general> $br
