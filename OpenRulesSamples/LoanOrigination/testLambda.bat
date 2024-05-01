@echo off
cd %~dp0
mvn openrules:testLambda -q -Dnochanges.skipUnitTest -DtestClassName=loan.origination.DecisionModelLoanOriginationTestClient -DtestUrl=https://ccfqf8bkzh.execute-api.us-east-1.amazonaws.com/test/loan-origination-result
