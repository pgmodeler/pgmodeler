# XML definition for view references
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
$tb <reference


#%if @{view} %then
#  [ view=] "@{view}" 
#%end

%if @{table} %then
  [ table=] "@{table}" 
  %if @{column} %then [ column=] "@{column}" %end
  %if @{alias} %then [ alias=] "@{alias}" %end
  %if @{column-alias} %then [ column-alias=] "@{column-alias}" %end
%else
  [ alias=] "@{alias}"
%end

%if @{expression} %then
 > $br
 $tb $tb <expression> <! $ob CDATA $ob @{expression} $cb $cb > </expression> $br
$tb </reference> $br
%else
 /> $br
%end
