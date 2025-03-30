#include <stdio.h>
#include <stdlib.h>

//#define SCRIPT_SIZE 80566
#define OFFSET 0
#define SCRIPT_SIZE 121404

int main(int argc, char *argv[])
{
    FILE *arq, *out;
    int c = 0;
    int cont = 0;
   /* char nome[11][20] = {
                                "SOMA CRUZ",
                                "MINA HAKUBA",
                                "ARIKADO",
                                "YOKO BELNADES",
                                "HAMMER",
                                "JULIUS BELMONT",
                                "CELIA FORTNER",
                                "DARIO BOSSI",                                
                                "DIMITRI BRINOFF",
                                "???"
                       };*/
                       
    char nome[17][20] = {
                           "Shanoa",
                           "Albus",
                           "Barlowe",
                           "Nikolai",
                           "Jacob",
                           "Abram",
                           "Laura",
                           "Eugen",
                           "Aeon",
                           "Marcel",
                           "George",
                           "Serge",
                           "Anna",
                           "Monica",
                           "Irina",
                           "Daniela",
                           "Dracula"
                        };
                       
    unsigned char *script = NULL;
    int i;

    unsigned char tabela[256] = " !\"#$%&`()@+,-./"
                                "0123456789:;<=>?"
                                "@ABCDEFGHIJKLMNO"
                                "PQRSTUVWXYZ[\\]^_"
                                "@abcdefghijklmno"
                                "pqrstuvwxyz{|}~@"
                                "@@@@@@@@@@@ÀÁÂÃ@"
                                "@@Ç@ÉÊ@@Í@@@Ñ@ÓÔ"
                                "Õ@@@@Ú@Ü@@àáâã@@"
                                "@ç@éê@@í@@@ñ@óôõ"
                                "@@@@ú@ü@@@@@@@@'"
                                "@@@@@@@@@@@@@@@@"
                                "@@@@@@@@@@@@@@@@"
                                "@@@@@@@@@@@@@@@@"
                                "@@@@@@*@@@@@@@@@"
                                "@@@@@@@@@@@@@@@@";

    if (argc != 2)
    {
        printf ("Usage: %s <file_to_dump>\n", argv[0]);
        exit (0);
    } 
	
	
    if ((arq = fopen (argv[1],"rb")) == NULL)
    {
        printf ("Erro na abertura do arquivo!\n");
        exit (0);        
    }

    if ((out = fopen ("script.txt","w")) == NULL)
    {
        printf ("Erro na abertura do arquivo!\n");
        exit (0);        
    }


    /* Dump comeca aqui. */
    
        
    fseek (arq, OFFSET, SEEK_SET);

    /* O script tem 80566 ou 0x13ABC caracteres */

    if ((script = (unsigned char*) malloc (sizeof(unsigned char)*SCRIPT_SIZE)) == NULL)
    {
        printf ("Erro na alocacao de memoria!\n");
        exit (0);        
    }


    if ((cont = fread (script, 1, SCRIPT_SIZE, arq)) != SCRIPT_SIZE)
    {
        printf ("Erro na leitura do arquivo!\n");        
        exit (0);        
    }

    i=0;

    while (i < SCRIPT_SIZE)
    {
        if (tabela[script[i]] != '@')
        {
            if (tabela[script[i]] == '*')
                fprintf(out, "%c\n", tabela[script[i]]);
            else
                fprintf(out, "%c", tabela[script[i]]);

            i++;
        }
        else
        {
            switch (script[i]) {
                case 0xE6:
                    fprintf(out, "*\n");
                    break;
                case 0xE3:
                    fprintf(out, "\n{%02X%02X} AVATAR\n", script[i], script[i+1]);
                    i++;
                    break;
                case 0xE7:
                    if (script[i+1] < 17)
                       fprintf(out, "\n{%02X%02X} %s\n", script[i], script[i+1], nome[script[i+1]]);
                    else
                        fprintf(out, "\n{%02X%02X} NOME\n", script[i], script[i+1]);
                    i++;
                    break;
                case 0xE5:
                    fprintf(out, "\n{%02X%02X} EVENTO\n", script[i], script[i+1]);
                    i++;
                    break;
                case 0xE2:
                    fprintf(out, "\n{%02X%02X} EVENTO\n", script[i], script[i+1]);
                    i++;
                    break;
                case 0xE1:
                    fprintf(out, "\n{%02X%02X%02X} EVENTO\n", script[i], script[i+1], script[i+2]);
                    i+=2;
                    break;
                case 0xEA:
                    fprintf(out, "\n\n---------------------------------------------\n\n");
                    break;
                default:
                    fprintf(out, "\n{%02X}\n", script[i]);
            }

            i++;
        }
    }

    fclose(arq);
    fclose(out);
    
    return 0;
}
