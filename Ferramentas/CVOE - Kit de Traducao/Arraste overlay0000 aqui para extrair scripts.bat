@echo off
if "%~1"=="" goto help
echo Extraindo script...
"%~dp0script_dumper" %1
echo Done!
goto end

:help
echo Uso: Arraste o overlay_0000.bin sobre este arquivo para extrair os scripts.

:end
echo.
pause
cls

