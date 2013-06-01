# XML definition for relationships
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<relationship name=] "@{name}" [ type=] "@{type}"

  %if @{src-col-pattern} %or @{dst-col-pattern} %then
    $br $tb
    %if @{src-col-pattern} %then [ src-col-pattern=] "@{src-col-pattern}" %end
    %if @{dst-col-pattern} %then [ dst-col-pattern=] "@{dst-col-pattern}" %end
  %end
  
  %if @{pk-pattern} %or @{uq-pattern} %then
    $br $tb
    %if @{pk-pattern} %then [ pk-pattern=] "@{pk-pattern}" %end
    %if @{uq-pattern} %then [ uq-pattern=] "@{uq-pattern}" %end
  %end

  %if @{src-fk-pattern} %or  @{dst-fk-pattern} %then
    $br $tb
    %if @{src-fk-pattern} %then [ src-fk-pattern=] "@{src-fk-pattern}" %end
    %if @{dst-fk-pattern} %then [ dst-fk-pattern=] "@{dst-fk-pattern}" %end
   %end

  %if @{col-indexes} %or  @{constr-indexes} %or @{attrib-indexes} %then
    $br $tb
    %if @{col-indexes} %then [ col-indexes=] "@{col-indexes}" %end
    %if @{attrib-indexes} %then [ attrib-indexes=] "@{attrib-indexes}" %end
    %if @{constr-indexes} %then [ constr-indexes=] "@{constr-indexes}" %end
   %end

  %if @{sql-disabled} %then
   $br $tb [ sql-disabled=] "true"
  %end

 %if @{protected} %then 
  $br [ protected=] "true"
 %end

$br $tb [ src-table=] "@{src-table}" 
$br $tb [ dst-table=] "@{dst-table}"


%if %not @{relgen} %and %not @{reldep} %then
  $br $tb
  [ src-required=] %if @{src-required} %then "true" %else "false" %end
  [ dst-required=] %if @{dst-required} %then "true" %else "false" %end

  %if @{identifier} %then
   $tb $sp identifier="true" $br
  %end

  %if @{table-name} %then
   $tb $sp table-name="@{table-name}"
  %end

  %if @{deferrable} %then
   $br
   $tb $sp deferrable="true"
   $sp defer-type="@{defer-type}"
  %end
%else
  %if @{copy-mode} %then
   $tb copy-mode="@{copy-mode}" $sp copy-options="@{copy-options}"
  %end
%end

%if @{reduced-form} %then
 /> $br $br
%else
 > $br

 %if @{points} %then $tb <line> $br $tb @{points} $tb </line> $br %end
 %if @{labels-pos} %then @{labels-pos} %end

 %if %not @{relgen} %and %not @{reldep} %then
   %if @{columns} %then @{columns} %end
   %if @{constraints} %then @{constraints} %end
 %end

 %if @{special-pk-cols} %then
   $tb [<special-pk-cols indexes=]"@{special-pk-cols}"/> $br
 %end

 </relationship>  $br $br
%end
