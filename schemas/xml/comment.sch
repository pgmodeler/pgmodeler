# XML definition for comments
# CAUTION: Do not modify this file unless you know what
#          you are doing.

#Creates <![CDATA[]]> element envolving the comment
#The tag <![CDATA[]]>  is converted to <! $ob CDATA $ob @{attribute} $cb > in order
#to not cause syntax errors on the schema parser

$tb <comment><! $ob CDATA $ob @{comment} $cb $cb ></comment> $br