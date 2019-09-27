%set {spacer} $br [            ]

[    <preset ] name="{name}"
%if {current-model} %then               
    {spacer} current-model="true"
%else
    {spacer} input-db="{input-db}"
%end

%if {compare-to-db} %then
    {spacer} compare-to-db="{compare-to-db}"
%end

%if {version} %then
    {spacer} version="{version}"
%end

%if {store-in-file} %then
    {spacer} store-in-file="true"
%else
    {spacer} apply-on-server="true"
%end

{spacer} keep-cluster-objs="{keep-cluster-objs}"
{spacer} keep-objs-perms="{keep-objs-perms}"
{spacer} dont-drop-missing-objs="{dont-drop-missing-objs}"
{spacer} drop-missing-cols-constrs="{drop-missing-cols-constrs}"
{spacer} preserve-db-name="{preserve-db-name}"
{spacer} drop-trunc-cascade="{drop-trunc-cascade}"
{spacer} trunc-cols-before-alter="{trunc-cols-before-alter}"
{spacer} force-objs-recreation="{force-objs-recreation}"
{spacer} recreate-unmod-objs="{recreate-unmod-objs}"
{spacer} import-sys-objs="{import-sys-objs}"
{spacer} import-ext-objs="{import-ext-objs}"
{spacer} reuse-sequences="{reuse-sequences}"
{spacer} ignore-import-errors="{ignore-import-errors}"
{spacer} ignore-duplic-errors="{ignore-duplic-errors}"

%if {ignore-error-codes} %then
    {spacer} ignore-error-codes="{ignore-error-codes}"
%end

/> $br $br
