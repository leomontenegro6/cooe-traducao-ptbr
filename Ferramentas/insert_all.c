#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_POINTERS 1893
#define TEXT_ADDRESS 0x138174
#define TABLE_ADDRESS 0x155BB4
#define MAX_SPLIT_POINT 0x155BB0
#define OVERLAY_SIZE 0x022B20
#define FINAL_TXT_ADDRESS 0x157954


struct pointer {
    unsigned char be_value[4];
    unsigned char le_value[4];
    unsigned int addr_le;
};

struct table_order {
    unsigned int index;
    unsigned int address;
};

struct script_bin {
       unsigned char *scr_bin;
       unsigned int size;
};

unsigned int chrToInt (const unsigned char *str, unsigned int num_of_chrs);
void sort(struct table_order array[], unsigned int begin, unsigned int end);


int main(int argc, char *argv[])
{
    FILE *f_rom, *f_scr;
    unsigned char s[100];
    int cont = 0;
    int c, k;
    unsigned int i, j;
    struct table_order ptr_order[NUM_POINTERS];
    unsigned int desvio;

    struct script_bin final_overlay;
    unsigned int split_point = 0;
    int num_of_split_ptrs = 0;


    unsigned int first_pointer = 0;
    
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

    struct pointer table[NUM_POINTERS], br_table[NUM_POINTERS];
    
    
    if (argc != 3)
    {
        printf ("Usage: %s <rom_file> <script_to_insert>\n", argv[0]);
        exit (0);
    } 
	
	
    if ((f_rom = fopen (argv[1],"rb+")) == NULL)
    {
        printf ("Erro na abertura do arquivo!\n");
        exit (0);        
    }

    if ((f_scr = fopen (argv[2],"r")) == NULL)
    {
        printf ("Erro na abertura do arquivo!\n");
        exit (0);        
    }

    /* Aloca buffer para os novos script e tabela traduzidos */
    final_overlay.scr_bin = NULL;
    final_overlay.scr_bin = (unsigned char*) malloc (sizeof(char)*OVERLAY_SIZE);
    if (final_overlay.scr_bin == NULL) {fputs ("Memory error",stderr); exit (2);}
    
    final_overlay.size = 0;
    
    while (fgets (s, 100, f_scr) != NULL)
    {
        if (!strcmp(s, "\n"))
        {
            continue;
        }
        else if (!strcmp(s, "---------------------------------------------\n"))
        {
            if (final_overlay.size >= OVERLAY_SIZE) {printf ("Espaco maximo de texto atingido. Reduza um pouco seu script.\n"); exit(0);}
             
            final_overlay.scr_bin[final_overlay.size] = 0xEA;
            final_overlay.size++;
            
            if (TEXT_ADDRESS + final_overlay.size <= MAX_SPLIT_POINT)
            {
               split_point = TEXT_ADDRESS + final_overlay.size;
            }
            else
            {
                num_of_split_ptrs++;
            }
        }
        else if (!strncmp(s, "{", 1))
        {
             
            if (s[3] == '}')
            {
                if (final_overlay.size >= OVERLAY_SIZE) {printf ("Espaco maximo de texto atingido. Reduza um pouco seu script.\n"); exit(0);}
                
                sscanf (&s[1], "%02X", &c);                
                final_overlay.scr_bin[final_overlay.size] = (unsigned char)c;
                final_overlay.size++;
            }
            else if (s[5] == '}')
            {
                if (final_overlay.size >= OVERLAY_SIZE) {printf ("Espaco maximo de texto atingido. Reduza um pouco seu script.\n"); exit(0);} 
                 
                sscanf (&s[1], "%02X", &c);
                final_overlay.scr_bin[final_overlay.size] = (unsigned char)c;
                final_overlay.size++;
                
                if (final_overlay.size >= OVERLAY_SIZE) {printf ("Espaco maximo de texto atingido. Reduza um pouco seu script.\n"); exit(0);}
                
                sscanf (&s[3], "%02X", &c);
                final_overlay.scr_bin[final_overlay.size] = (unsigned char)c;
                final_overlay.size++;
            }
            else if (s[7] == '}')
            {
                if (final_overlay.size >= OVERLAY_SIZE) {printf ("Espaco maximo de texto atingido. Reduza um pouco seu script.\n"); exit(0);}
                 
                sscanf (&s[1], "%02X", &c);
                final_overlay.scr_bin[final_overlay.size] = (unsigned char)c;
                final_overlay.size++;
                
                if (final_overlay.size >= OVERLAY_SIZE) {printf ("Espaco maximo de texto atingido. Reduza um pouco seu script.\n"); exit(0);}
                
                sscanf (&s[3], "%02X", &c);
                final_overlay.scr_bin[final_overlay.size] = (unsigned char)c;
                final_overlay.size++;
                
                if (final_overlay.size >= OVERLAY_SIZE) {printf ("Espaco maximo de texto atingido. Reduza um pouco seu script.\n"); exit(0);}
                
                sscanf (&s[5], "%02X", &c);
                final_overlay.scr_bin[final_overlay.size] = (unsigned char)c;
                final_overlay.size++;
            }
        }
        else
        {
            for (k=0; k<(int)strlen(s)-1; k++)
            {
                c = s[k];
                cont = 0;

                while (tabela[cont] != c) cont++;
                
                if (final_overlay.size >= OVERLAY_SIZE) {printf ("Espaco maximo de texto atingido. Reduza um pouco seu script! :P\n"); exit(0);}
                
                final_overlay.scr_bin[final_overlay.size] = (unsigned char)cont;
                final_overlay.size++;
            }
        }          
    }
    
    
    /* Segunda Parte */
    
    //printf ("split point = %06X\n", split_point);
    //exit(0);
    
    
    /* criacao da tabela de ponteiros comeca aqui.*/

    fseek (f_rom, TABLE_ADDRESS, SEEK_SET);

    /* Leitura da tabela original */
    for (i=0; i<NUM_POINTERS; i++)
    {
        table[i].be_value[0] = table[i].le_value[3] = (unsigned char) fgetc(f_rom);
        table[i].be_value[1] = table[i].le_value[2] = (unsigned char) fgetc(f_rom);
        table[i].be_value[2] = table[i].le_value[1] = (unsigned char) fgetc(f_rom);
        table[i].be_value[3] = table[i].le_value[0] = (unsigned char) fgetc(f_rom);

        ptr_order[i].index = i;
        ptr_order[i].address = chrToInt(table[i].le_value, 4);
        
        /* Obtendo o ponteiro para a primeira posicao do script */
        if (ptr_order[first_pointer].address > ptr_order[i].address)
           first_pointer = i;
    }

    /* Calculo do desvio e ordenamento dos vetores da tabela */
    desvio = chrToInt(table[first_pointer].le_value, 4) - TEXT_ADDRESS;
    
    printf("desvio = %06X\n", desvio);

    /* Ordenando a sequencia de ponteiros de acordo com a ordem do script */
    sort(ptr_order, 0, NUM_POINTERS);    
    
    // Recalculando os ponteiros para a nova tabela
    j = 0;
    
    for (i=0; i<NUM_POINTERS; i++)
    {
        int first = 0;
           
        while (final_overlay.scr_bin[j] != 0xEA)
        {
            if ((final_overlay.scr_bin[j] == 0x01 && final_overlay.scr_bin[j+1] == 0x00) && !first)
            {
                br_table[ptr_order[i].index].be_value[0] = br_table[ptr_order[i].index].le_value[3] =  ((j + desvio + TEXT_ADDRESS) & 0x000000FF);
                br_table[ptr_order[i].index].be_value[1] = br_table[ptr_order[i].index].le_value[2] = (((j + desvio + TEXT_ADDRESS) & 0x0000FF00)>>8);
                br_table[ptr_order[i].index].be_value[2] = br_table[ptr_order[i].index].le_value[1] = (((j + desvio + TEXT_ADDRESS) & 0x00FF0000)>>16);
                br_table[ptr_order[i].index].be_value[3] = br_table[ptr_order[i].index].le_value[0] = (((j + desvio + TEXT_ADDRESS) & 0xFF000000)>>24);
                
                br_table[ptr_order[i].index].addr_le = j + desvio + TEXT_ADDRESS;
                
                first = 1;

                /*if (i>1285)
                {
                    printf("%04d %02X %02X %02X %02X\n", ptr_order[i].index, br_table[ptr_order[i].index].le_value[0],
                                                    br_table[ptr_order[i].index].le_value[1],
                                                    br_table[ptr_order[i].index].le_value[2],
                                                    br_table[ptr_order[i].index].le_value[3]);
                }*/
            }
            
            j++;              
        }
        
        j++;
    }
    
    
    /* Terceira Parte */
    
    /*  */
    
    if (split_point < MAX_SPLIT_POINT)
    {
        /* Codigo com split aqui. */
        for (i=NUM_POINTERS-num_of_split_ptrs; i<NUM_POINTERS; i++)
        {
            printf ("num of splits = %d\n", num_of_split_ptrs);
            
            br_table[ptr_order[i].index].addr_le = br_table[ptr_order[i].index].addr_le + FINAL_TXT_ADDRESS - split_point;
            
            br_table[ptr_order[i].index].be_value[0] = br_table[ptr_order[i].index].le_value[3] =  (br_table[ptr_order[i].index].addr_le & 0x000000FF);
            br_table[ptr_order[i].index].be_value[1] = br_table[ptr_order[i].index].le_value[2] = ((br_table[ptr_order[i].index].addr_le & 0x0000FF00)>>8);
            br_table[ptr_order[i].index].be_value[2] = br_table[ptr_order[i].index].le_value[1] = ((br_table[ptr_order[i].index].addr_le & 0x00FF0000)>>16);
            br_table[ptr_order[i].index].be_value[3] = br_table[ptr_order[i].index].le_value[0] = ((br_table[ptr_order[i].index].addr_le & 0xFF000000)>>24);                       
        }
        
        
        fseek (f_rom, TEXT_ADDRESS, SEEK_SET);
        
        for (i=0; i<split_point - TEXT_ADDRESS; i++)
        {
            fputc (final_overlay.scr_bin[i], f_rom);
        }
        
        for (i=0; i<TABLE_ADDRESS - split_point; i++)
        {
            fputc (0, f_rom);
        }
        
        fseek (f_rom, TABLE_ADDRESS, SEEK_SET);
        
        for (i=0; i<NUM_POINTERS; i++)
        {
            fputc(br_table[i].be_value[0], f_rom);
            fputc(br_table[i].be_value[1], f_rom);
            fputc(br_table[i].be_value[2], f_rom);
            fputc(br_table[i].be_value[3], f_rom);
        }
        
        fseek (f_rom, FINAL_TXT_ADDRESS, SEEK_SET);    
        
        for (i=split_point - TEXT_ADDRESS; i<final_overlay.size; i++)
        {
            fputc (final_overlay.scr_bin[i], f_rom);
        }
        
        for (i=0; i<4; i++)
        {
            fputc (0, f_rom);
        }
    }
    else
    {
        /* Codigo sem split aqui. */
        fseek (f_rom, TEXT_ADDRESS, SEEK_SET);
        
        for (i=0; i<final_overlay.size; i++)
        {
            fputc (final_overlay.scr_bin[i], f_rom);
        }
        
        for (i=0; i<TABLE_ADDRESS - TEXT_ADDRESS - final_overlay.size; i++)
        {
            fputc (0, f_rom);
        }
        
        fseek (f_rom, TABLE_ADDRESS, SEEK_SET);
        
        for (i=0; i<NUM_POINTERS; i++)
        {
            fputc(br_table[i].be_value[0], f_rom);
            fputc(br_table[i].be_value[1], f_rom);
            fputc(br_table[i].be_value[2], f_rom);
            fputc(br_table[i].be_value[3], f_rom);
        }        
    }

    free (final_overlay.scr_bin);
    
    fclose(f_rom);
    fclose(f_scr);
    
    return 0;
}


unsigned int chrToInt (const unsigned char *str, unsigned int num_of_chrs)
{
    unsigned int num = 0;
    unsigned int i;

    for (i=0; i<num_of_chrs; i++)
    {
        num = num + ((unsigned int)str[i])*((unsigned int)pow(16, 2*(num_of_chrs-i-1)));        
    }

    return num;
}

void sort(struct table_order array[], unsigned int begin, unsigned int end) {
   unsigned int pivot = array[begin].address;
   unsigned int i = begin + 1, j = end, k = end;
   struct table_order t;

   while (i < j) {
      if (array[i].address < pivot) i++;
      else if (array[i].address > pivot) {
         j--; k--;
         t = array[i];
         array[i] = array[j];
         array[j] = array[k];
         array[k] = t; }
      else {
         j--;         
         t = array[i];
         array[i] = array[j];
         array[j] = t;
   }  }
   i--;
   t = array[begin];
   array[begin] = array[i];
   array[i] = t;
   if (i - begin > 1)
      sort(array, begin, i);
   if (end - k   > 1)
      sort(array, k, end);                      
}
