@echo off
cd %~dp0
call mvn openrules:testREST -DtestClassName=vacation.days.VacationDaysTestClient -DtestUrl=http://localhost:8080/custom-vacation-days
pause
