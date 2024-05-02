@echo off
cd %~dp0
call mvn openrules:testREST -DtestClassName=vacation.days.rest.VacationDaysModelTestClient -DtestUrl=http://localhost:8080/vacation-days-model -e
pause
