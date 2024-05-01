@echo off
cd %~dp0
call mvn openrules:testLambda -q -DskipLocalTests -DtestClassName=promotion.DecisionModelOrderPromotionTestClient -DtestUrl=https://p13xt7kdlc.execute-api.us-east-1.amazonaws.com/test/decision-model-order-promotion
pause
