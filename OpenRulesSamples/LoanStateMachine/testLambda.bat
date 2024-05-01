@echo off
cd %~dp0
call mvn openrules:testLambda -q -DskipLocalTests -DtestClassName=loan.LoanStateMachineTestClient -DtestUrl=https://vc639emga5.execute-api.us-east-1.amazonaws.com/test/loan-state-machine
pause
