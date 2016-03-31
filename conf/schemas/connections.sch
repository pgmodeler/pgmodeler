# XML definition for connections configuration
# CAUTION: Do not modify this file unless you know what you are doing.
%if {connections} %then

[<?xml version="1.0" encoding="UTF-8" ?>] $br
[<!--] $br
[  CAUTION: Do not modify this file directly on it's code unless you know what you are doing.] $br
[           Unexpected results may occur if the code is changed deliberately.] $br
[-->] $br

<connections> $br
{connections} $br
</connections> $br

%else

$tb [<connection alias="] {alias} [" host=] "{host}" [ port=] "{port}" [ dbname=] "{dbname}" $br
$tb $tb [user=] "{user}"

%if {password} %then [ password=] "{password}" %end

[ connection-timeout=] "{connection-timeout}" $br

$tb $tb [sslmode=] "{sslmode}" $br

%if {sslcert} %then $tb $tb [sslcert=] "{sslcert}" $br %end
%if {sslkey} %then  $tb $tb [sslkey=] "{sslkey}" $br %end 
%if {sslrootcert} %then $tb $tb [sslrootcert=] "{sslrootcert}" $br %end 
%if {sslcrl} %then $tb $tb [sslcrl=] "{sslcrl}" $br %end


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


/> $br
%end
