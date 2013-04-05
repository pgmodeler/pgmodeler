# SQL definition for operator class elements
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
%if @{function} %then
 $tb FUNCTION $tb @{stg-number} $tb @{signature}
%end

%if @{operator} %then
 $tb OPERATOR $tb @{stg-number} $tb @{signature}

 %if %not @{pgsql90} %and @{opfamily} %then
    %if @{for-order-by} %then
	[ FOR ORDER BY ]
    %else
	[ FOR SEARCH ]
    %end
    @{opfamily}
 %end
%end

%if @{storage} %then
 $tb STORAGE $tb @{type}
%end
