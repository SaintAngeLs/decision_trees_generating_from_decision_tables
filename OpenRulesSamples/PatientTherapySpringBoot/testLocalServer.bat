@echo off
cd %~dp0
call mvn openrules:testREST -DtestClassName=tax.calculator.springboot.TaxCalculatorTestClient -DtestUrl=http://localhost:8080/tax-calculator
pause
