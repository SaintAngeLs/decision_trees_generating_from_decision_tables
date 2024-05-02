@echo off
cd %~dp0

docker run -p 8080:8080 -t medical-service-coverage
