# XML definition for prepended SQL commands
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

#Creates <![CDATA[]]> element envolving the prepended-sql
#The tag <![CDATA[]]> is converted to <! $ob CDATA $ob {attribute} $cb > in order
#to not cause syntax errors on the schema parser

$tb <prepended-sql> $sp <! $ob CDATA $ob {prepended-sql} $cb $cb > $sp </prepended-sql> $br
