#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *arq, *out;
    unsigned char s[100];
    int cont = 0;
    int c;
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
        printf ("Usage: %s <file_to_insert>\n", argv[0]);
        exit (0);
    } 
	
	
    if ((arq = fopen (argv[1],"r")) == NULL)
    {
        printf ("Erro na abertura do arquivo!\n");
        exit (0);        
    }

    if ((out = fopen ("script.bin","wb")) == NULL)
    {
        printf ("Erro na abertura do arquivo!\n");
        exit (0);        
    }


    /* Insercao comeca aqui. */


    /* O script tem 80566 ou 0x13ABC caracteres */

    
    while (fgets (s, 100, arq) != NULL)
    {
        if (!strcmp(s, "\n"))
        {
            continue;
        }
        else if (!strcmp(s, "---------------------------------------------\n"))
        {
            c = 0xEA;
            fputc (c, out);
        }
        else if (!strncmp(s, "{", 1))
        {
            if (s[3] == '}')
            {
                sscanf (&s[1], "%02X", &c);
                fputc (c, out);
            }
            else if (s[5] == '}')
            {
                sscanf (&s[1], "%02X", &c);
                fputc (c, out);
                sscanf (&s[3], "%02X", &c);
                fputc (c, out);
            }
            else if (s[7] == '}')
            {
                sscanf (&s[1], "%02X", &c);
                fputc (c, out);
                sscanf (&s[3], "%02X", &c);
                fputc (c, out);
                sscanf (&s[5], "%02X", &c);
                fputc (c, out);
            }
        }
        else
        {

            for (i=0; i<(int)strlen(s)-1; i++)
            {
                c = s[i];
                cont = 0;

                while (tabela[cont] != c) cont++;

                fputc (cont, out);

            }
        }          
    } 

    fclose(arq);
    fclose(out);
    
    return 0;
}
