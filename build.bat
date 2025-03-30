@echo off

del ".\cooe.nds"
copy ".\orig-expandida.nds" ".\cooe.nds"

echo ==Inserindo script na rom...
".\Ferramentas\CVOE - Kit de Traducao\insert_all.exe" ".\cooe.nds" ".\Scripts\script.txt"

echo ==Inserindo graficos na rom...
rmdir rom_desmontada /q /s
md rom_desmontada
cd rom_desmontada
..\Ferramentas\ndstool.exe -x ..\cooe.nds -9 arm9.bin -7 arm7.bin -y9 y9.bin -y7 y7.bin -d data -y overlay -t banner.bin -h header.bin
xcopy ..\Graficos\Editados\* . /e /y
..\Ferramentas\ndstool.exe -c ..\cooe.nds -9 arm9.bin -7 arm7.bin -y9 y9.bin -y7 y7.bin -d data -y overlay -t banner.bin -h header.bin
cd ..
rmdir rom_desmontada /q /s

echo ==Destrimmar a rom, deixando-a com 64mb...
".\Ferramentas\armips.exe" ".\Asm\expansor_rom.asm"