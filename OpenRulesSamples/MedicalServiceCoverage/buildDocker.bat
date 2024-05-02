@echo off
cd %~dp0

call mvn compile jib:dockerBuild -Dimage=medical-service-coverage
pause