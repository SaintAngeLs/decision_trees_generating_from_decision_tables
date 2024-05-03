@echo off
cd %~dp0
call mvn test -Dopenrules.error=trace
pause
