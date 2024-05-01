@echo off
cd %~dp0
call mvn test -q
pause
