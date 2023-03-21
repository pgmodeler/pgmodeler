# SQL definition for operator class elements
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.
%if {function} %then
	$tb FUNCTION $tb {stg-number} $tb {signature}
%end

%if {operator} %then
	$tb OPERATOR $tb {stg-number} $tb {signature}

	%if {opfamily} %then
		[ FOR ORDER BY ] {opfamily}
	%else
		[ FOR SEARCH ]
	%end
%end

%if {storage} %then
	$tb STORAGE $tb {type}
%end
