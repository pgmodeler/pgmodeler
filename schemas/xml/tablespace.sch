# XML definition for tablespaces
# CAUTION: Do not modify this file unless you know what
#          you are doing.
%if @{reduced-form} %then $tb %end
[<tablespace name=] "@{name}"

%if @{reduced-form} %then
/>  $br
%else
  [ directory=] "@{directory}" 

  %if @{protected} %then 
   [ protected=] "true"
  %end 

  > $br


  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end
  </tablespace>  $br $br
%end