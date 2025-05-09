# XML definition for connections configuration
# CAUTION: Do not modify this file unless you know what you are doing.
%if {connections} %then

	[<?xml version="1.0" encoding="UTF-8" ?>] \n
	[<!--] \n
	[ CAUTION: Do not modify this file directly on it's code unless you know what you are doing.] \n
	[ Unexpected results may occur if the code is changed deliberately.] \n
	[-->] \n

	<connections> \n

	{connections}

	</connections> \n

%else

	%set {spc} \n \t

	\t [<connection alias="] &{alias} [" host=] "&{host}" [ port=] "{port}" [ dbname=] "&{dbname}"

	{spc} [user=] "&{user}"

	%if {password} %then [ password=] "&{password}" %end

	[ connection-timeout=] "{connection-timeout}"

	%if {setrole} %then [ setrole=] "&{setrole}" %end

	{spc} [sslmode=] "{sslmode}"

	%if {sslcert} %then {spc} [sslcert=] "&{sslcert}" %end
	%if {sslkey} %then {spc} [sslkey=] "&{sslkey}" %end
	%if {sslrootcert} %then {spc} [sslrootcert=] "&{sslrootcert}" %end
	%if {sslcrl} %then {spc} [sslcrl=] "&{sslcrl}" %end

	%if {krbsrvname} %then
		 {spc} [krbsrvname=] "&{krbsrvname}"

		%if {gsslib} %then
			[ gsslib=] "{gsslib}"
		%end
	%end

	%if {auto-browse-db} %then
		{spc} auto-browse-db="true"
	%end

	%if {options} %then
		{spc} options="&{options}"
	%end

	%if {default-for-export} %then {spc} [default-for-export=] "{default-for-export}" %end
	%if {default-for-import} %then {spc} [default-for-import=] "{default-for-import}" %end
	%if {default-for-diff} %then  {spc} [default-for-diff=] "{default-for-diff}" %end
	%if {default-for-validation} %then {spc} [default-for-validation=] "{default-for-validation}" %end    

	/> \n
%end
