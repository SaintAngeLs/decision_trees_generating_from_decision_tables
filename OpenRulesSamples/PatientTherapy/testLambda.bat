@echo off
cd %~dp0
call mvn openrules:testLambda -q -DskipLocalTests -DtestClassName=healthcare.DecisionModelPatientTherapyTestClient -DtestUrl=https://6w1d1ohn8k.execute-api.us-east-1.amazonaws.com/test/decision-model-patient-therapy
pause
