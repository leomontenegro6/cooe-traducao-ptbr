@echo off

del ".\cooe.nds"

echo ==Aplicando patch de expansao de overlay na rom original...
cd ".\Ferramentas\CVOE - Kit de expansao\"
.\xdelta.exe -dfs ..\..\orig.nds .\cvoe.pat ..\..\cooe.nds
cd ..\..\

echo ==Inserindo script na rom expandida...
".\Ferramentas\CVOE - Kit de Traducao\insert_all.exe" ".\cooe.nds" ".\Scripts\script.txt"

echo ==Inserindo graficos na rom expandida...
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