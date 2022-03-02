# XML definition for basic configurations file
# CAUTION: Do not modify this file unless you know what you are doing.
[<?xml version="1.0" encoding="UTF-8" ?>] $br
[<!--] $br
[ CAUTION: Do not modify this file directly on it's code unless you know what you are doing.] $br
[ Unexpected results may occur if the code is changed deliberately.] $br
[-->] $br

<general> $br
$tb [<configuration op-list-size="] {op-list-size} ["] $br
$tb $tb [grid-size="] {grid-size} ["] $br
$tb $tb [grid-color="] {grid-color} ["] $br
$tb $tb [canvas-color="] {canvas-color} ["] $br
$tb $tb [delimiters-color="] {delimiters-color} ["] $br
$tb $tb [autosave-interval="] {autosave-interval} ["] $br
$tb $tb [paper-type="] {paper-type} ["] $br
$tb $tb [paper-orientation="] {paper-orientation} ["] $br
$tb $tb [paper-margin="] {paper-margin} ["] $br

%if {paper-custom-size} %then
	$tb $tb [paper-custom-size="] {paper-custom-size} ["] $br
%end

$tb $tb [print-grid="] %if {print-grid} %then true %else false %end ["] $br
$tb $tb [print-pg-num="] %if {print-pg-num} %then true %else false %end ["] $br
$tb $tb [hide-rel-name="] %if {hide-rel-name} %then true %else false %end ["] $br
$tb $tb [hide-ext-attribs="] %if {hide-ext-attribs} %then true %else false %end ["] $br
$tb $tb [hide-table-tags="] %if {hide-table-tags} %then true %else false %end ["] $br
$tb $tb [code-font="] {code-font} ["] $br
$tb $tb [code-font-size="] {code-font-size} ["] $br
$tb $tb [code-tab-width="] {code-tab-width} ["] $br
$tb $tb [canvas-corner-move="] %if {canvas-corner-move} %then true %else false %end ["] $br
$tb $tb [invert-rangesel-trigger="] %if {invert-rangesel-trigger} %then true %else false %end ["] $br
$tb $tb [check-update="] %if {check-update} %then true %else false %end ["] $br
$tb $tb [check-versions="] {check-versions} ["] $br
$tb $tb [save-last-position="] %if {save-last-position} %then true %else false %end ["] $br
$tb $tb [show-main-menu="] %if {show-main-menu} %then true %else false %end ["] $br
$tb $tb [disable-smoothness="] %if {disable-smoothness} %then true %else false %end ["] $br
$tb $tb [simplified-obj-creation="] %if {simplified-obj-creation} %then true %else false %end ["] $br
$tb $tb [confirm-validation="] %if {confirm-validation} %then true %else false %end ["] $br
$tb $tb [code-completion="] %if {code-completion} %then true %else false %end ["] $br
$tb $tb [display-line-numbers="] %if {display-line-numbers} %then true %else false %end ["] $br
$tb $tb [highlight-lines="] %if {highlight-lines} %then true %else false %end ["] $br
$tb $tb [line-numbers-color="] {line-numbers-color} ["] $br
$tb $tb [line-numbers-bg-color="] {line-numbers-bg-color} ["] $br
$tb $tb [line-highlight-color="] {line-highlight-color} ["] $br
$tb $tb [use-placeholders="] %if {use-placeholders} %then true %else false %end ["] $br
$tb $tb [show-canvas-grid="] %if {show-canvas-grid} %then true %else false %end ["] $br
$tb $tb [show-page-delimiters="] %if {show-page-delimiters} %then true %else false %end ["] $br
$tb $tb [align-objs-to-grid="] %if {align-objs-to-grid} %then true %else false %end ["] $br
$tb $tb [min-object-opacity="] {min-object-opacity} ["] $br
$tb $tb [attribs-per-page="] {attribs-per-page} ["] $br
$tb $tb [ext-attribs-per-page="] {ext-attribs-per-page} ["] $br
$tb $tb [history-max-length="] {history-max-length} ["] $br
$tb $tb [use-curved-lines="] %if {use-curved-lines} %then true %else false %end ["] $br
$tb $tb [compact-view="] %if {compact-view} %then true %else false %end ["] $br
$tb $tb [save-restore-geometry="] %if {save-restore-geometry} %then true %else false %end ["] $br
$tb $tb [low-verbosity="] %if {low-verbosity} %then true %else false %end ["] $br
$tb $tb [escape-comment="] %if {escape-comment} %then true %else false %end ["] $br
$tb $tb [hide-schema-names-of-types="] %if {hide-schema-names-of-types} %then true %else false %end ["] $br

%if {source-editor-app} %then
	$tb $tb [source-editor-app="] {source-editor-app} ["] $br
%end

%if {source-editor-app} %and {source-editor-args} %then
	$tb $tb [source-editor-args="] {source-editor-args} ["] $br
%end

%if {ui-language} %then
	$tb $tb [ui-language="] {ui-language} ["] $br
%end

$tb /> $br

%if {file} %then
	$tb <session> $br
	$tb {file}
	$tb </session> $br
%end

%if {recent-models} %then
	$tb <recent-models> $br
	$tb {recent-models}
	$tb </recent-models> $br
%end

%if {dock-widgets} %then
	$tb <dock-widgets> $br
	$tb {dock-widgets}
	$tb </dock-widgets> $br
%end

%if {widgets-geometry} %then
	$tb <widgets-geometry> $br
	$tb {widgets-geometry}
	$tb </widgets-geometry> $br
%end

</general> $br
