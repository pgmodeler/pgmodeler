# SQL definition for role members changes
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {revoke} %then
	[REVOKE ]

	%if {admin-option} %then 
		[ADMIN OPTION FOR ]
	%end
%else
	[GRANT ]
%end

{role}

%if {revoke} %then
	[ FROM ]
%else
	[ TO ] 
%end

{roles}

%if %not {revoke} %and {admin-option} %then 
	[ WITH ADMIN OPTION]
%end

%if {revoke} %then 
	[ CASCADE] 
%end

; $br [-- ddl-end --] $br
