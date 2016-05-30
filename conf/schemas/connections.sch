# XML definition for connections configuration
# CAUTION: Do not modify this file unless you know what you are doing.
%if {connections} %then

[<?xml version="1.0" encoding="UTF-8" ?>] $br
[<!--] $br
[  CAUTION: Do not modify this file directly on it's code unless you know what you are doing.] $br
[           Unexpected results may occur if the code is changed deliberately.] $br
[-->] $br

<connections> $br
{connections}
</connections> $br

%else

$tb [<connection alias="] {alias} [" host=] "{host}" [ port=] "{port}" [ dbname=] "{dbname}" $br
$tb $tb [user=] "{user}"

%if {password} %then [ password=] "{password}" %end

[ connection-timeout=] "{connection-timeout}" 

$br $tb $tb [sslmode=] "{sslmode}"

%if {sslcert} %then $br $tb $tb [sslcert=] "{sslcert}" %end
%if {sslkey} %then  $br $tb $tb [sslkey=] "{sslkey}"  %end 
%if {sslrootcert} %then $br $tb $tb [sslrootcert=] "{sslrootcert}"  %end 
%if {sslcrl} %then $br $tb $tb [sslcrl=] "{sslcrl}"  %end


%if {krbsrvname} %then
 $br $tb $tb [krbsrvname=] "{krbsrvname}" 

 %if {gsslib} %then 
   [ gsslib=] "{gsslib}"
 %end
%end

%if {auto-browse-db} %then
 $br $tb $tb auto-browse-db="true"
%end

%if {options} %then
 $br $tb $tb options="{options}"
%end

%if {default-for-export} %then $br $tb $tb [default-for-export=] "{default-for-export}" %end
%if {default-for-import} %then $br $tb $tb [default-for-import=] "{default-for-import}" %end
%if {default-for-diff} %then $br $tb $tb [default-for-diff=] "{default-for-diff}" %end
%if {default-for-validation} %then $br $tb $tb [default-for-validation=] "{default-for-validation}" %end

/> $br
%end
