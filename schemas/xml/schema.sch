# XML definition for schemas
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
%if @{reduced-form} %then $tb %end
[<schema name=] "@{name}"

%if @{reduced-form} %then
/> $br
%else

  %if @{protected} %then 
   [ protected=] "true"
  %end

  %if @{rect-visible} %then 
   [ rect-visible=] "true"
  %end

  %if @{fill-color} %then 
   [ fill-color=] "@{fill-color}"
  %end

  %if @{sql-disabled} %then
   [ sql-disabled=] "true"
  %end
  
  > $br

  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end

  </schema>  $br $br
%end
