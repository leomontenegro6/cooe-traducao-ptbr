; Script de expansão da rom pra 64mb
.gba
.open "cooe.nds", 0x0
.orga filesize("cooe.nds")
.fill 67108864 - filesize("cooe.nds"), 0xff
.close