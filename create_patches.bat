@echo off
del /Q ".\Patches\cooe_1.1.xdelta"
".\Ferramentas\xdelta.exe" -e -s ".\orig.nds" ".\cooe.nds" ".\Patches\cooe_1.1.xdelta"