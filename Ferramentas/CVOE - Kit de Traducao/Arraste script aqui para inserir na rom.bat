@echo off
if "%~1"=="" goto help
echo Inserindo script...
"%~dp0insert_all" "%~dp0cvoexpbr.nds" %1
echo Done!
goto end

:help
echo Uso: Arraste o script sobre este arquivo para inserir na rom.
echo Renomeie sua rom para cvoexpbr.nds

:end
echo.
pause
cls

