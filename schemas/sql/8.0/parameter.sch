# SQL definition for function parameters
# PostgreSQL Version: 8.0
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
%if @{reduced-form} %then
 $sp
%else
 $sp @{name} $sp
%end

@{type}, $sp
