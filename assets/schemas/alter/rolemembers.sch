# SQL definition for role members changes
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {revoke} %then
	[REVOKE ]

	%if {admin-option} %then 
		[ADMIN OPTION ]
	%end
%else
	[GRANT ]
%end

{roles}

%if {revoke} %then
	[ FROM ]
%else
	[ TO ] 
%end

{role}

%if %not {revoke} %and {admin-option} %then 
	[ WITH ADMIN OPTION ]
%end

%if {revoke} %and {cascade} %then 
	[ CASCADE] 
%end

#GRANT role_name [, ...] TO role_specification [, ...]
#    [ WITH ADMIN OPTION ]
#    [ GRANTED BY role_specification ]

#REVOKE [ ADMIN OPTION FOR ]
#    role_name [, ...] FROM role_specification [, ...]
#    [ GRANTED BY role_specification ]
 #   [ CASCADE | RESTRICT ]