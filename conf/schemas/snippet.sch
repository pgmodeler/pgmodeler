# XML definition for snippets configurations file
# CAUTION: Do not modify this file unless you know what you are doing.
$tb [<snippet id=] "@{id}" [ label=] "@{label}" [ object=] "@{object}" > $br
$tb <! $ob CDATA $ob $br
@{contents}
$br $tb $cb $cb > $br
$tb </snippet> $br