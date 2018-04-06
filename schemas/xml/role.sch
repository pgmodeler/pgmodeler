# XML definition for roles
# CAUTION: Do not modify this file unless you know what you are doing.
#          Code generation can be broken if incorrect changes are made.
%if {reduced-form} %then $tb %end
[<role name=] "{name}"

%if {reduced-form} %then
/> $br
%else
 %if {superuser} %then $br [      superuser="true"]  %end
 %if {createdb} %then $br [      createdb="true"] %end
 %if {replication} %then $br [      replication="true"] %end
 %if {createrole} %then $br [      createrole="true"] %end
 %if {inherit} %then $br [      inherit="true"] %end
 %if {login} %then $br [      login="true"] %end
 %if {bypassrls} %then $br [      bypassrls="true"] %end
 %if {encrypted} %then $br [      encrypted="true"] %end
 %if {connlimit} %then $br [      connlimit=]"{connlimit}" %end
 %if {validity} %then $br [      validity=]"{validity}" %end
 %if {password} %then $br [      password=]"{password}" %end
 %if {protected} %then 
  $br [      protected="true"]
 %end

 %if {sql-disabled} %then
  $br [      sql-disabled="true"]
 %end
 > $br

 %if {comment} %then {comment} %end
 %if {appended-sql} %then {appended-sql} %end
 %if {prepended-sql} %then {prepended-sql} %end

 %if {ref-roles} %then $tb [<roles names=] "{ref-roles}" [ role-type="refer" />] $br  %end
 %if {member-roles} %then $tb [<roles names=] "{member-roles}" [ role-type="member" />] $br  %end
 %if {admin-roles} %then $tb [<roles names=] "{admin-roles}" [ role-type="admin" />] $br %end

 </role> $br $br
%end
