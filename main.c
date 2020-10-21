#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dirent.h>
#include <string.h>

int main(void) {
    typedef struct listnode
    {

        char* same_item_string;                 //ena stng me ta koina items metaksu istotopwn (orizontia)
        struct node *next;
    }list_node;

    typedef struct contenttable
    {
        list_node  *list_ptr;
        char* item_address;    //e.g amazon /ebay /ktl
        int potition;                 //arithmos theshs ston pinaka me ola ta records twn istotopwn
    }content_table;

    /* //// gia tuxon new data
     llist **new_array;
     new_array = realloc(array, new_size * sizeof(LList*));
     */
    /*    ///file  count lines mallon de xreiazetai
    FILE *fp;

    char filename[".json"];
    char c;

    printf("Enter file name: ");
    scanf("%s", filename);

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Could not open file %s", filename);
        return 0;
    }

    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n')
            count = count + 1;

    fclose(fp);
    printf("The file %s has %d lines\n ", filename, count);*/
//////////////////////////////////////////////////////////////////////
    int count = 0;
    int i ;
    struct dirent *de;  // pointer sto directory


    DIR *dr = opendir("C:\\Users\\g.kabardis\\Downloads\\");      // opendir() =epistrefei pointer tou directory.

    if (dr == NULL)  //
    {
        printf("Could not open current directory" );
        return 0;
    }

    while ((de = readdir(dr)) != NULL)             //mporoume na paiksoume me readdir?!!!! diavasma arxeiwn katalogou gia malloc array
    {
        count++;
       // printf("%s\n", de->d_name); //DIR NAME

    }

    closedir(dr);
///////////////////////////////////////////
    printf( "%d \n", count);


    /*  list_node * head = NULL;
  head = (list_node *) malloc(sizeof(list_node));
    if (head == NULL)
    {
        return 1;
    }*/

    content_table array_contents[count] ;
    for (i=0;i>=count;i++)
    {
        array_contents[i].list_ptr = NULL;
        array_contents[i].list_ptr = (list_node *) malloc(sizeof(list_node));

        /*if (array_contents[i].list_ptr == NULL) {
            return 1;
        }*/
        array_contents[i].item_address=NULL;
        array_contents[i].potition=0;

    }
    //char str[500000000000];
    ///////////////////////////////////////////////////////////////

    //Llist * array;   //pinakas apo deiktes pou deixnoun se llist nodes
    //array = malloc(count * sizeof(Llist*));

    ///////////////////////////////////
   opendir(dr);      // opendir() =epistrefei pointer tou directory.
    i=0;
    if (dr == NULL)  //
    {
        printf("Could not open current directory" );
        return 0;
    }

    while ((de = readdir(dr)) != NULL)
    {
        array_contents[i].item_address = de->d_name;
        array_contents[i].potition=i;
        array_contents[i].list_ptr->same_item_string=de->d_name;
        i++;
    }

    closedir(dr);



        FILE* stream = fopen("test.csv","r"); //diavasma excel gia dhmiourgia LISTWN

        char line[1024];
        while (fgets(line, 1024, stream))
        {
            char* tmp = strdup(line);
            char* tok;
            for (tok = strtok(line, ",");
                 tok && *tok;
                 tok = strtok(NULL, ",\n"))
            {
                if (tok!=NULL && tok != "0")
                {
                    printf("Field 3 is 1.MAKE LIST  %s\n");

                }
                free(tmp);
            }



        }

    /*TA GRAFW OLA SE MAIN GIATI EIMAI DOULEIA.PROFANWS THELOUN ORGANWSH SE HEADERS,.C KTL.
      MEXRI TWRA DIAVASAME DIRECTORIES GIA NA VROUME POSA EINAI SUNOLO (EBAY/AMAZON/KTL )KAI
       FTIAXNOUME ENA PINAKA APO STRUCTS DEIKTWN SE LINKED LISTS ME PLHTHOS AUTO ,TWN DIRECTORIES.
      KATHE DIRECTORY ADISTOIXEI SE MIA THESH STON ARRAY ,OPOTE KSANADIAVASAME GIA NA VALOUME SE KATHE THESH TOU ARRAY NAME APO PROION
       KAI ADISTOIXIA ME TON EUATO TOU.
       META DIAVASAME TO EXCEL KAI THA FTIAXTOUN
      OI KOMVOI THS LISTAS KAI ANALOGA ME TO AN YPARXEI 1 STO 3o PEDIO KATHE GRAMMHS TOU EXCELL .EDW PAIZEITAI OLO TO PRAGMA
      ME TO PWS THA KANOUME TIS SUGKRISEIS APODOTIKA NA MHN KSANAGURNAME PISW SUNEXEIA .EPISHS ISWS H PRWTH DOMH STRUCT ARRAY ISWS PREPEI NA GINEI DENTRO*/


    return 0;

}
