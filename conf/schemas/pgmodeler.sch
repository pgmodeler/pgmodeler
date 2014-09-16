# XML definition for basic configurations file
# CAUTION: Do not modify this file unless you know what you are doing.
[<?xml version="1.0" encoding="UTF-8" ?>] $br
[<!--] $br
[  CAUTION: Do not modify this file directly on it's code unless you know what you are doing.] $br
[           Unexpected results may occur if the code is changed deliberately.] $br
[-->] $br

<pgmodeler> $br
$sp [<configuration op-list-size="] @{op-list-size} ["] $br
[               grid-size="] @{grid-size} ["] $br
[               autosave-interval="] @{autosave-interval} ["] $br
[               paper-type="] @{paper-type} ["] $br
[               paper-orientation="] @{paper-orientation} ["] $br
[               paper-margin="] @{paper-margin} ["] $br

%if @{paper-custom-size} %then
[               paper-custom-size="] @{paper-custom-size} ["] $br
%end

[               print-grid="] %if @{print-grid} %then true %else false %end ["] $br
[               print-pg-num="] %if @{print-pg-num} %then true %else false %end ["] $br
[               hide-rel-name="] %if @{hide-rel-name} %then true %else false %end ["] $br
[               hide-ext-attribs="] %if @{hide-ext-attribs} %then true %else false %end ["] $br
[               hide-table-tags="] %if @{hide-table-tags} %then true %else false %end ["] $br
[               code-font="] @{code-font} ["] $br
[               code-font-size="] @{code-font-size} ["] $br
[               canvas-corner-move="] %if @{canvas-corner-move} %then true %else false %end ["] $br
[               invert-panning-rangesel="] %if @{invert-panning-rangesel} %then true %else false %end ["] $br
[               check-update="] %if @{check-update} %then true %else false %end ["] $br
[               save-last-position="] %if @{save-last-position} %then true %else false %end ["] $br
[               show-main-menu="] %if @{show-main-menu} %then true %else false %end ["] $br
[               disable-smoothness="] %if @{disable-smoothness} %then true %else false %end ["] $br
[               simplified-obj-creation="] %if @{simplified-obj-creation} %then true %else false %end ["]
[/>] $br

%if @{file} %then
 $sp <session> $br
 $sp @{file}
 $sp </session> $br
%end

%if @{recent-models} %then
 $sp <recent-models> $br
 $sp @{recent-models}
 $sp </recent-models> $br
%end

%if @{dock-widgets} %then
 $sp <dock-widgets> $br
 $sp @{dock-widgets}
 $sp </dock-widgets> $br
%end

</pgmodeler> $br
