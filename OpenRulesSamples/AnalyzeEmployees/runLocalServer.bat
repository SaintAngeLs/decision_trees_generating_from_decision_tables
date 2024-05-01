@echo off
cd %~dp0
call mvn openrules:runRest 
pause
