@echo off
cd %~dp0
call mvn openrules:testLambda -q -DskipLocalTests -DtestClassName=credit.card.application.DMCreditCardApplicationTestClient -DtestUrl=https://ki9o0o16ed.execute-api.us-east-1.amazonaws.com/test/d-m-credit-card-application
pause
