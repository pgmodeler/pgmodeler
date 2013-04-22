# XML definition for operator families
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
%if @{reduced-form} %then $tb %end
[<opfamily name=] "@{name}"

%if @{reduced-form} %then
/> $br
%else
  [ index-type=] "@{index-type}" 

  %if @{protected} %then 
   [ protected=] "true"
  %end

  %if @{sql-disabled} %then
   [ sql-disabled=] "true"
  %end

  > $br

  @{schema}
  %if @{owner} %then @{owner} %end
  %if @{comment} %then @{comment} %end
  </opfamily>  $br $br
%end
