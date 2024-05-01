@echo off
cd %~dp0
call mvn openrules:testLambda -q -DskipLocalTests -DtestClassName=org.dmcommunity.challenge.ICD10TestClient -DtestUrl=https://393a6tx7oa.execute-api.us-east-1.amazonaws.com/test/i-c-d10
pause
