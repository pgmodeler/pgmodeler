# XML definition for comments
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

#Creates <![CDATA[]]> element envolving the comment
#The tag <![CDATA[]]> is converted to <! $ob CDATA $ob {attribute} $cb > in order
#to not cause syntax errors on the schema parser

$tb <comment> $sp <! $ob CDATA $ob {comment} $cb $cb > $sp </comment> $br