@echo off
cd %~dp0
call mvn spring-boot:run -e
pause