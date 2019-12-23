# XML definition for basic configurations file
# CAUTION: Do not modify this file unless you know what you are doing.
[<?xml version="1.0" encoding="UTF-8" ?>] $br
[<!--] $br
[  CAUTION: Do not modify this file directly on it's code unless you know what you are doing.] $br
[           Unexpected results may occur if the code is changed deliberately.] $br
[-->] $br

<pgmodeler> $br
$sp [<configuration op-list-size="] {op-list-size} ["] $br
[               grid-size="] {grid-size} ["] $br
[               autosave-interval="] {autosave-interval} ["] $br
[               paper-type="] {paper-type} ["] $br
[               paper-orientation="] {paper-orientation} ["] $br
[               paper-margin="] {paper-margin} ["] $br

%if {paper-custom-size} %then
[               paper-custom-size="] {paper-custom-size} ["] $br
%end

[               print-grid="] %if {print-grid} %then true %else false %end ["] $br
[               print-pg-num="] %if {print-pg-num} %then true %else false %end ["] $br
[               hide-rel-name="] %if {hide-rel-name} %then true %else false %end ["] $br
[               hide-ext-attribs="] %if {hide-ext-attribs} %then true %else false %end ["] $br
[               hide-table-tags="] %if {hide-table-tags} %then true %else false %end ["] $br
[               code-font="] {code-font} ["] $br
[               code-font-size="] {code-font-size} ["] $br
[               code-tab-width="] {code-tab-width} ["] $br
[               canvas-corner-move="] %if {canvas-corner-move} %then true %else false %end ["] $br
[               invert-rangesel-trigger="] %if {invert-rangesel-trigger} %then true %else false %end ["] $br
[               check-update="] %if {check-update} %then true %else false %end ["] $br
[               save-last-position="] %if {save-last-position} %then true %else false %end ["] $br
[               show-main-menu="] %if {show-main-menu} %then true %else false %end ["] $br
[               disable-smoothness="] %if {disable-smoothness} %then true %else false %end ["] $br
[               simplified-obj-creation="] %if {simplified-obj-creation} %then true %else false %end ["] $br
[               confirm-validation="] %if {confirm-validation} %then true %else false %end ["] $br
[               code-completion="] %if {code-completion} %then true %else false %end ["] $br
[               display-line-numbers="] %if {display-line-numbers} %then true %else false %end ["] $br
[               highlight-lines="] %if {highlight-lines} %then true %else false %end ["] $br
[               line-numbers-color="] {line-numbers-color} ["] $br
[               line-numbers-bg-color="] {line-numbers-bg-color} ["] $br
[               line-highlight-color="] {line-highlight-color} ["] $br
[               use-placeholders="] %if {use-placeholders} %then true %else false %end ["] $br
[               show-canvas-grid="] %if {show-canvas-grid} %then true %else false %end ["] $br
[               show-page-delimiters="] %if {show-page-delimiters} %then true %else false %end ["] $br
[               align-objs-to-grid="] %if {align-objs-to-grid} %then true %else false %end ["] $br
[               min-object-opacity="] {min-object-opacity} ["] $br
[               attribs-per-page="] {attribs-per-page} ["] $br
[               ext-attribs-per-page="] {ext-attribs-per-page} ["] $br
[               history-max-length="] {history-max-length} ["] $br
[               use-curved-lines="] %if {use-curved-lines} %then true %else false %end ["] $br
[               compact-view="] %if {compact-view} %then true %else false %end ["] $br
[               save-restore-geometry="] %if {save-restore-geometry} %then true %else false %end ["] $br
[               low-verbosity="] %if {low-verbosity} %then true %else false %end ["] $br
[               escape-comment="] %if {escape-comment} %then true %else false %end ["] $br

%if {source-editor-app} %then
[               source-editor-app="] {source-editor-app} ["] $br
%end

%if {source-editor-app} %and {source-editor-args} %then
[               source-editor-args="] {source-editor-args} ["] $br
%end

%if {ui-language} %then
[               ui-language="] {ui-language} ["] $br
%end

[/>] $br

%if {file} %then
 $sp <session> $br
 $sp {file}
 $sp </session> $br
%end

%if {recent-models} %then
 $sp <recent-models> $br
 $sp {recent-models}
 $sp </recent-models> $br
%end

%if {dock-widgets} %then
 $sp <dock-widgets> $br
 $sp {dock-widgets}
 $sp </dock-widgets> $br
%end

%if {widgets-geometry} %then
 $sp <widgets-geometry> $br
 $sp {widgets-geometry}
 $sp </widgets-geometry> $br
%end

</pgmodeler> $br
