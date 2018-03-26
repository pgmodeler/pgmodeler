# XML definition for extensions
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<extension name=] "{name}"
  %if {handles-type} %then
    [ handles-type="true"]
  %end

  %if {cur-version} %then
    [ cur-version=]"{cur-version}"
  %end

  %if {old-version} %then
    [ old-version=]"{old-version}"
  %end

  %if {protected} %then 
   [ protected=] "true"
  %end

  %if {sql-disabled} %then
   [ sql-disabled=] "true"
  %end

  > $br

  %if {schema} %then
    {schema}
  %end

  %if {comment} %then {comment} %end
  %if {appended-sql} %then {appended-sql} %end
  %if {prepended-sql} %then {prepended-sql} %end

</extension>  $br $br
 
