# XML definition for relationships configuration file
# CAUTION: Do not modify this file unless you know what you are doing.
[<?xml version="1.0" encoding="UTF-8" ?>] $br
[<!--] $br
[ CAUTION: Do not modify this file directly on it's code unless you know what you are doing.] $br
[ Unexpected results may occur if the code is changed deliberately.] $br
[-->] $br

<relationships $sp link-mode="{link-mode}" > $br
    
$tb [<foreign-keys deferrable=] "{deferrable}" 

[ defer-type=] "{defer-type}" 
[ upd-action=] "{upd-action}" 
[ del-action=] "{del-action}" 
[ fk-idx-type=] "{fk-idx-type}"

[/>] $br

$tb <name-patterns> $br
{patterns}
$tb </name-patterns> $br

</relationships> $br

