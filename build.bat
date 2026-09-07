@echo off
echo ==Gerando rom traduzida.==

set versao=original
IF "%1" == "l" SET versao=latinfix

del ".\cooe.nds"

echo ==Aplicando patch de expansao de overlay na rom original...
".\Ferramentas\xdelta.exe" -dfs ".\orig.nds" ".\Ferramentas\CVOE - Kit de expansao\cvoe.pat" ".\cooe.nds"

echo ==Inserindo script na rom expandida...
IF "%versao%" == "latinfix" (
    ".\Ferramentas\CVOE - Kit de Traducao\insert_all.exe" ".\cooe.nds" ".\Scripts\script_latinfix.txt"
) ELSE (
    ".\Ferramentas\CVOE - Kit de Traducao\insert_all.exe" ".\cooe.nds" ".\Scripts\script.txt"
)

echo ==Inserindo graficos na rom expandida...
rmdir rom_desmontada /q /s
md rom_desmontada
cd rom_desmontada
..\Ferramentas\ndstool.exe -x ..\cooe.nds -9 arm9.bin -7 arm7.bin -y9 y9.bin -y7 y7.bin -d data -y overlay -t banner.bin -h header.bin
xcopy ..\Graficos\Editados\* . /e /y
IF "%versao%" == "latinfix" (
    xcopy "..\Graficos\Editados (latinfix)\*" . /e /y
)
..\Ferramentas\ndstool.exe -c ..\cooe.nds -9 arm9.bin -7 arm7.bin -y9 y9.bin -y7 y7.bin -d data -y overlay -t banner.bin -h header.bin
cd ..
rmdir rom_desmontada /q /s

echo ==Destrimmar a rom, deixando-a com 64mb...
".\Ferramentas\armips.exe" ".\Asm\expansor_rom.asm"