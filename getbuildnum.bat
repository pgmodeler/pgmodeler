@echo off
setlocal
rem Generates a build number based upon the current local date (YYYYMMDD)
rem Source: https://superuser.com/questions/1287756/how-can-i-get-the-date-in-a-locale-independent-format-in-a-batch-file
rem use findstr to strip blank lines from wmic output
for /f "usebackq skip=1 tokens=1-3" %%g in (`wmic Path Win32_LocalTime Get Day^,Month^,Year ^| findstr /r /v "^$"`) do (
  set _day=00%%g
  set _month=00%%h
  set _year=%%i
  )
rem Pad the day and month with leading zeroes
set _month=%_month:~-2%
set _day=%_day:~-2%
echo %_year%%_month%%_day%
endlocal
