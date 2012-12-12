# SQL definition for function parameters
# PostgreSQL Version: 8.x, 9.0
# CAUTION: Do not modify this file unless you know what
#          you are doing.
%if @{in} %then IN  %end
%if @{out} %then OUT  %end
$sp @{name} $sp
$sp @{type}, $sp
