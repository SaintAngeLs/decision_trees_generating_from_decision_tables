@echo off
cd %~dp0
call mvnw openrules:testLambda -q -DskipLocalTests -DtestClassName=org.medical.services.MedicalServiceCoverageTestClient -DtestUrl=https://ha2yaaa1w5.execute-api.us-east-1.amazonaws.com/test/medical-service-coverage
pause
