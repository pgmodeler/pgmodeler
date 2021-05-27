# XML definition for grants
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
<permission

%if {revoke} %then
 [ revoke="true"]
 %if {cascade} %then
   [ cascade="true"]
 %end
%end

  %if {sql-disabled} %then
   [ sql-disabled=] "true"
  %end

> $br


$tb [<object name=] "{object}" 

 %if {parent} %then  [ parent=] "{parent}"  %end

 [ type=] "{type}" [/>] $br

 %if {roles} %then
  $tb [<roles names=] "{roles}" [/>] $br
 %end

 $tb [<privileges]

 %if {select} %then [ select=] "{select}" %end
 %if {delete} %then [ delete=] "{delete}" %end
 %if {insert} %then [ insert=] "{insert}" %end
 %if {update} %then [ update=] "{update}" %end
 %if {truncate} %then [ truncate=] "{truncate}" %end
 %if {references} %then [ references=] "{references}" %end
 %if {trigger} %then [ trigger=] "{trigger}" %end
 %if {create} %then [ create=] "{create}" %end
 %if {connect} %then [ connect=] "{connect}" %end
 %if {temporary} %then [ temporary=] "{temporary}" %end
 %if {execute} %then [ execute=] "{execute}" %end
 %if {usage} %then [ usage=] "{usage}" %end

 [/>] $br

</permission> $br
