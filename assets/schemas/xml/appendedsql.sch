# XML definition for appendend SQL commands
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

#Creates <![CDATA[]]> element envolving the appended-sql
#The tag <![CDATA[]]> is converted to <! $ob CDATA $ob {attribute} $cb > in order
#to not cause syntax errors on the schema parser

$tb <appended-sql> $sp <! $ob CDATA $ob {appended-sql} $cb $cb > $sp </appended-sql> $br
