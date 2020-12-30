# XML definition for a single object's metadata
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
$tb [<info object=] "{name}" [ type=] "{type}" 

%if {tag} %then
    [ tag=] "{tag}"
%end

%if {protected} %or {sql-disabled}  %then
    $br $tb $tb
    %if {protected} %then [ protected="true"] %end
    %if {sql-disabled} %then [ sql-disabled="true"] %end
%end

%if {author} %or {last-position} %or {last-zoom} %then
    $br $tb $tb
    %if {author} %then [ author=] "{author}" %end
    %if {last-position} %then [ last-position=] "{last-position}" %end
    %if {last-zoom} %then  [ last-zoom=] "{last-zoom}" %end
%end

%if {append-at-eod} %or {prepend-at-bod}  %then
    $br $tb $tb
    %if {append-at-eod} %then [ append-at-eod=] "{append-at-eod}" %end
    %if {prepend-at-bod} %then [ prepend-at-bod=] "{prepend-at-bod}" %end
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

%if {faded-out} %or {collapse-mode} %then
    $br $tb $tb
    %if {faded-out} %then [ faded-out=] "{faded-out}" %end
    %if {collapse-mode} %then [ collapse-mode=] "{collapse-mode}" %end
%end

%if {alias} %then
    $br $tb $tb
    %if {alias} %then [ alias=] "{alias}" %end
    %if {table} %then [ table=] "{table}" %end
%end
    
%if {z-value} %then
    $br $tb $tb [ z-value=] "{z-value}"
%end

%if {position} %or {appended-sql} %or {prepended-sql} %then
    > $br
    %if {position} %then {position} %end
    %if {appended-sql} %then $tb {appended-sql} %end
    %if {prepended-sql} %then $tb {prepended-sql} %end 
    $tb </info> 
%else
    [/>]
%end

$br
