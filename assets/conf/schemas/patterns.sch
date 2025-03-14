# XML definition for relationship name patterns (part of relationships.conf file)
# CAUTION: Do not modify this file unless you know what you are doing.

\t \t [<patterns type=] "{type}"

%if {src-col-pattern} %then [ src-col-pattern=] "&{src-col-pattern}" %end
%if {dst-col-pattern} %then [ dst-col-pattern=] "&{dst-col-pattern}" %end
%if {src-fk-pattern} %then [ src-fk-pattern=] "&{src-fk-pattern}" %end
%if {dst-fk-pattern} %then [ dst-fk-pattern=] "&{dst-fk-pattern}" %end
%if {pk-pattern} %then [ pk-pattern=] "&{pk-pattern}" %end
%if {uq-pattern} %then [ uq-pattern=] "&{uq-pattern}" %end
%if {pk-col-pattern} %then [ pk-col-pattern=] "&{pk-col-pattern}" %end
%if {fk-idx-pattern} %then [ fk-idx-pattern=] "&{fk-idx-pattern}" %end

[/>] \n
