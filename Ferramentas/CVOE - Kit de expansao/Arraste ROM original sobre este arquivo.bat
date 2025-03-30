@echo off
if "%~1"=="" goto help
echo Patching...
"%~dp0xdelta" -dfs %1 "%~dp0cvoe.pat" "%~dpn1-patched%~x1"
if %ERRORLEVEL%==9009 goto missingxdelta
if %ERRORLEVEL%==1 goto patchfailed
echo Done! The patched file has been created in the same folder as the original.
echo If the patch does not work, make sure that you applied it to the original,
echo unpatched, untrimmed ROM.
goto end

:missingxdelta
echo.
echo Unable to find the xdelta patching utility. Make sure that you extract all
echo of the patch files to the same folder before running this.
goto end

:patchfailed
echo.
echo The xdelta patching utility was unable to apply the patch. Make sure to use the
echo original (unpatched, untrimmed) ROM and extract all of the patch files to the
echo same folder before running this.
goto end

:help
echo You're almost there, but don't try to open the Drop Rom Here file by itself.
echo To patch your game, close this window and drag the original (unpatched,
echo untrimmed) ROM onto the Drop Rom Here file. The patched ROM will be created in
echo the same folder as the original, with a different name.

:end
echo.
pause
cls

