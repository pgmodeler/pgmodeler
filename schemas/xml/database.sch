# XML definition for tablespaces
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
[<database name=] "@{name}"

%if @{encoding} %then [ encoding=] "@{encoding}" %end
%if @{template} %then [ template=] "@{template}" %end
%if @{connlimit} %then [ connlimit=] "@{connlimit}" %end
%if @{lc-collate} %then [ lc-collate=] "@{lc-collate}" %end
%if @{lc-ctype} %then [ lc-ctype=] "@{lc-ctype}" %end

  %if @{sql-disabled} %then
   [ sql-disabled=] "true"
  %end

  %if @{append-at-eod} %then
   [ append-at-eod=] "true"
  %end

> $br

%if @{owner} %then @{owner} %end
%if @{tablespace} %then @{tablespace} %end
%if @{comment} %then @{comment} %end
%if @{appended-sql} %then @{appended-sql} %end

</database>  $br $br
