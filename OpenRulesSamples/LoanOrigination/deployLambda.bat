@echo off
cd %~dp0
mvn openrules:deployLambda -e
