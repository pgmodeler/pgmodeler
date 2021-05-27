# SQL definition for domain's constraints change
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

[ALTER ] {sql-object} $sp {signature} 

 %if ({expression} == "unset") %then
   [ DROP CONSTRAINT ]
 %else
   [ ADD CONSTRAINT ]
 %end

 %if {name} %then
   {name}
 %end

 %if ({expression} != "unset") %then
   $sp CHECK $sp ({expression})
 %end
 
; 

$br [-- ddl-end --] $br
