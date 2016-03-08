# XML definition for files opened on a session
# CAUTION: Do not modify this file unless you know what you are doing.
$tb [<widget]

%if {validator} %then
  [ id="validator"]
  [ sql-validation="] %if {sql-validation} %then true %else false %end ["]
  [ use-unique-names="] %if {use-unique-names} %then true %else false %end ["]
  [ pgsql-version="] %if {pgsql-ver} %then {pgsql-ver} %else auto %end ["]
%else
  %if {objectfinder} %then
    [ id="objectfinder"]
    [ regexp="] %if {regexp} %then true %else false %end ["]
    [ case-sensitive="] %if {case-sensitive} %then true %else false %end ["]
    [ exact-match="] %if {exact-match} %then true %else false %end ["]
    [ highlight-objects="] %if {highlight-objects} %then true %else false %end ["]
  %else 
    %if {sqltool} %then
      [ id="sqltool"]
      [ show-attributes-grid="] %if {show-attributes-grid} %then true %else false %end ["]
      [ show-source-pane="] %if {show-source-pane} %then true %else false %end ["]
    %end
  %end
%end

/> $br
