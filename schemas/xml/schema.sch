# XML definition for schemas
# CAUTION: Do not modify this file unless you know what
#          you are doing.
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
  
  > $br

  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end

  </schema>  $br $br
%end