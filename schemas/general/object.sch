# XML definition for object positioning
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
$tb [<object name=] "{name}" [ type=] "{type}" 

%if {tag} %then
    [ tag=] "{tag}"
%end

%if {sql-disabled} %or {protected} %then
    $br $tb $tb
    %if {sql-disable} %then [ sql-disabled="true"] %end
    %if {protected} %then [ protected="true"] %end
%end

%if {author} %or {last-position} %or {last-zoom} %then
    $br $tb $tb
    %if {author} %then [ author=] "{author}" %end
    %if {last-position} %then [ last-position=] "{last-position}" %end
    %if {last-zoom} %then  [ last-zoom=] "{last-zoom}" %end
%end

%if {default-schema} %or {default-owner}  %then
    $br $tb $tb
    %if {default-schema} %then [ default-schema=] "{default-schema}" %end
    %if {default-owner} %then  [ default-owner=] "{default-owner}" %end
%end

%if {default-collation} %or {default-tablespace} %then
    $br $tb $tb
    %if {default-collation} %then [ default-collation=] "{default-collation}" %end
    %if {default-tablespace} %then [ default-tablespace=] "{default-tablespace}" %end
%end

%if {src-table} %then
    $br $tb $tb  [ src-table=] "{src-table}" [ src-type=] "{src-type}"
%end

%if {dst-table} %then
    $br $tb $tb  [ dst-table=] "{dst-table}" [ dst-type=] "{dst-type}"
%end

%if {custom-color} %or {rect-visible} %then
    $br $tb $tb
    %if {custom-color} %then [ custom-color=] "{custom-color}" %end
    %if {rect-visible} %then [ rect-visible=] "{rect-visible}" %end
%end

%if {position} %or {appended-sql} %or {prepended-sql} %then
    > $br
    %if {position} %then {position} %end
    %if {appended-sql} %then $tb {appended-sql} %end
    %if {prepended-sql} %then $tb {prepended-sql} %end 
    $tb </object> 
%else
    [/>]
%end

$br
