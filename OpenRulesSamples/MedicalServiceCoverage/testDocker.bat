@echo off
cd %~dp0
call mvn compile exec:java -Dexec.mainClass=org.medical.services.MedicalServiceCoverageTestClient -DtestUrl=http://localhost:8080/medical-service-coverage

pause
