{ddl-end}

%if {owner} %then {owner} %end
%if {comment} %then {comment} %end

%if {appended-sql} %then
	{appended-sql}
	{ddl-end}
%end

$br