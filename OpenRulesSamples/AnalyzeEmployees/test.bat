@echo off
cd %~dp0
call mvn clean test -q
pause
