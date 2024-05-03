@echo off
cd %~dp0
call mvn openrules:testLambda -q -DskipLocalTests -DtestClassName=aggregated.DecisionModelAggregatedValuesTestClient -DtestUrl=https://6502zjtlib.execute-api.us-east-1.amazonaws.com/test/decision-model-aggregated-values
pause
