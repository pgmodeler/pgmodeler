# XML definition for domains
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<domain name=] "@{name}"

%if @{reduced-form} %then
  /> $br
%else
 %if @{constraint} %then
   [ constraint=] "@{constraint}"
  %end

  [ not-null=] %if @{not-null} %then "true" %else "false" %end
  %if @{default-value} %then [ default=] "@{default-value}" %end

  %if @{protected} %then 
   [ protected=] "true"
  %end

  %if @{sql-disabled} %then
   [ sql-disabled=] "true"
  %end

  > $br

  @{schema}
  %if @{owner} %then @{owner} %end
  %if @{collation} %then @{collation} %end
  %if @{comment} %then @{comment} %end
  %if @{appended-sql} %then @{appended-sql} %end

  $tb @{type}

  %if @{expression} %then
   $tb <expression> <! $ob CDATA $ob @{expression} $cb $cb > </expression>$br
  %end

  </domain>  $br $br
%end
