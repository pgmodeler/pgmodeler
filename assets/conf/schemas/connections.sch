# XML definition for connections configuration
# CAUTION: Do not modify this file unless you know what you are doing.
%if {connections} %then

	[<?xml version="1.0" encoding="UTF-8" ?>] $br
	[<!--] $br
	[ CAUTION: Do not modify this file directly on it's code unless you know what you are doing.] $br
	[ Unexpected results may occur if the code is changed deliberately.] $br
	[-->] $br

	<connections> $br

	{connections}

	</connections> $br

%else

	%set {spc} $br $br

	$tb [<connection alias="] {alias} [" host=] "{host}" [ port=] "{port}" [ dbname=] "{dbname}" $br
	{spc} [user=] "{user}"

	%if {password} %then [ password=] "{password}" %end

	[ connection-timeout=] "{connection-timeout}"

	$br {spc} [sslmode=] "{sslmode}"

	%if {sslcert} %then $br {spc} [sslcert=] "{sslcert}" %end
	%if {sslkey} %then $br {spc} [sslkey=] "{sslkey}" %end
	%if {sslrootcert} %then $br {spc} [sslrootcert=] "{sslrootcert}" %end
	%if {sslcrl} %then $br {spc} [sslcrl=] "{sslcrl}" %end

	%if {krbsrvname} %then
		$br {spc} [krbsrvname=] "{krbsrvname}"

		%if {gsslib} %then
			[ gsslib=] "{gsslib}"
		%end
	%end

	%if {auto-browse-db} %then
		$br {spc} auto-browse-db="true"
	%end

	%if {options} %then
		$br {spc} options="{options}"
	%end

	%if {default-for-export} %then $br {spc} [default-for-export=] "{default-for-export}" %end
	%if {default-for-import} %then $br {spc} [default-for-import=] "{default-for-import}" %end
	%if {default-for-diff} %then $br {spc} [default-for-diff=] "{default-for-diff}" %end
	%if {default-for-validation} %then $br {spc} [default-for-validation=] "{default-for-validation}" %end    

	/> $br
%end
