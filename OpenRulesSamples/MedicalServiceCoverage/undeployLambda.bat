@echo off
cd %~dp0
call mvn openrules:undeployLambda
pause
