@echo off
REM This script is a workaround for lack of a decent 'date' command on Windows.
REM It returns the date in format YYYYMMDD (locale aware) used as build number.
for /F "usebackq tokens=1,2 delims==" %%i in (`wmic os get LocalDateTime /VALUE 2^>NUL`) do if '.%%i.'=='.LocalDateTime.' set date=%%j
set date=%date:~0,4%%date:~4,2%%date:~6,2%
echo %date%