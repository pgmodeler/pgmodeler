# XML definition for object tag
# CAUTION: Do not modify this file unless you know what you are doing.
# Code generation can be broken if incorrect changes are made.

%if {reduced-form} %then
	$tb
%end

[<tag name=] "&{name}"

%if {reduced-form} %then
	/> $br
%else
	%if {protected} %then
		[ protected=] "true"
	%end

	> $br

	{styles}

	%if {comment} %then
		{comment}
	%end

	</tag> $br
%end
