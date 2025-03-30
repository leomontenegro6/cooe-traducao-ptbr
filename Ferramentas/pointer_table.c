#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_POINTERS 1893
#define TEXT_ADDRESS 0
#define TABLE_ADDRESS 0x01DA40

struct pointer {
    unsigned char be_value[4];
    unsigned char le_value[4];
    unsigned int addr_le;
};

struct table_order {
    unsigned int index;
    unsigned int address;
};

unsigned int chrToInt (const unsigned char *str, unsigned int num_of_chrs);
void sort(struct table_order array[], unsigned int begin, unsigned int end);


int main(int argc, char *argv[])
{
    FILE *ori, *br, *out;
    int cont = 0;
    unsigned int i, j;
    struct table_order ptr_order[NUM_POINTERS];
    unsigned int desvio;

    long scriptbr_size;
    unsigned char *scriptbr;
    size_t result;
    unsigned int first_pointer = 0;

    struct pointer table[NUM_POINTERS], br_table[NUM_POINTERS];
    
    
    //unsigned char teste[5];

    if (argc != 3)
    {
        printf ("Usage: %s <original_file> <br_file>\n", argv[0]);
        exit (0);
    } 
	
	
    if ((ori = fopen (argv[1],"rb")) == NULL)
    {
        printf ("Erro na abertura do arquivo!\n");
        exit (0);        
    }

    if ((br = fopen (argv[2],"rb")) == NULL)
    {
        printf ("Erro na abertura do arquivo!\n");
        exit (0);        
    }

    if ((out = fopen ("table_br.bin","wb")) == NULL)
    {
        printf ("Erro na abertura do arquivo!\n");
        exit (0);        
    }

    /* criacao da tabela de ponteiros comeca aqui.*/

    fseek (ori, TABLE_ADDRESS, SEEK_SET);

    /* Leitura da tabela original */
    for (i=0; i<NUM_POINTERS; i++)
    {
        table[i].be_value[0] = table[i].le_value[3] = (unsigned char) fgetc(ori);
        table[i].be_value[1] = table[i].le_value[2] = (unsigned char) fgetc(ori);
        table[i].be_value[2] = table[i].le_value[1] = (unsigned char) fgetc(ori);
        table[i].be_value[3] = table[i].le_value[0] = (unsigned char) fgetc(ori);

        ptr_order[i].index = i;
        ptr_order[i].address = chrToInt(table[i].le_value, 4);
        
        /* Obtendo o ponteiro para a primeira posicao do script */
        if (ptr_order[first_pointer].address > ptr_order[i].address)
           first_pointer = i;
    }

    /* Calculo do desvio e ordenamento dos vetores da tabela */
    desvio = chrToInt(table[first_pointer].le_value, 4) - TEXT_ADDRESS;
    
    //printf("desvio = %06X\n", desvio);
    
    //printf("primeiro ponteiro = %d, %08X\n", first_pointer, ptr_order[first_pointer].address);
    

    /* Ordenando a sequencia de ponteiros de acordo com a ordem do script */
    sort(ptr_order, 0, NUM_POINTERS);

    // obtem tamanho do arquivo:
    fseek (br, 0, SEEK_END);
    scriptbr_size = ftell (br);
    rewind (br);

    // aloca memoria para conter o arquivo inteiro:
    scriptbr = (unsigned char*) malloc (sizeof(char)*scriptbr_size);
    if (scriptbr == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copia o arquivo para o buffer:
    result = fread (scriptbr, 1, scriptbr_size, br);
    if (result != scriptbr_size) {fputs ("Reading error.\n", stderr); exit (3);}
    
    // Recalculando os ponteiros para a nova tabela
    j = 0;
    
    for (i=0; i<NUM_POINTERS; i++)
    {
        int first = 0;
           
        while (scriptbr[j] != 0xEA)
        {
            if ((scriptbr[j] == 0x01 && scriptbr[j+1] == 0x00) && !first)
            {
                br_table[ptr_order[i].index].be_value[0] = br_table[ptr_order[i].index].le_value[3] =  ((j + desvio + TEXT_ADDRESS) & 0x000000FF);
                br_table[ptr_order[i].index].be_value[1] = br_table[ptr_order[i].index].le_value[2] = (((j + desvio + TEXT_ADDRESS) & 0x0000FF00)>>8);
                br_table[ptr_order[i].index].be_value[2] = br_table[ptr_order[i].index].le_value[1] = (((j + desvio + TEXT_ADDRESS) & 0x00FF0000)>>16);
                br_table[ptr_order[i].index].be_value[3] = br_table[ptr_order[i].index].le_value[0] = (((j + desvio + TEXT_ADDRESS) & 0xFF000000)>>24);
                
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
    
    
    // Salvando a nova tabela de ponteiros.
    for (i=0; i<NUM_POINTERS; i++)
    {
        fputc(br_table[i].be_value[0], out);
        fputc(br_table[i].be_value[1], out);
        fputc(br_table[i].be_value[2], out);
        fputc(br_table[i].be_value[3], out);
    }

    fclose(ori);
    fclose(br);
    fclose(out);
    
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
