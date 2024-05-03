@echo off
cd %~dp0
call mvn openrules:deployLambda -e
pause
