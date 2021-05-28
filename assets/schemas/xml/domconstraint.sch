# XML definition for domain's check constraint
# PostgreSQL Version: 9.x
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.

$tb [<constraint name="] {name} [" type="check">] $br
$tb $tb <expression> <! $ob CDATA $ob {expression} $cb $cb > </expression> $br
$tb [</constraint>] $br 
