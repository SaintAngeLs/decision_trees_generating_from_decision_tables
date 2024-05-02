@echo off
cd %~dp0
call mvn openrules:testLambda -q -DskipLocalTests -DtestClassName=team.performance.DecisionModelTeamPerformanceTestClient -DtestUrl=https://8dthqs0evg.execute-api.us-east-1.amazonaws.com/test/decision-model-team-performance
pause
