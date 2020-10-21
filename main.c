#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dirent.h>

int main(void) {
    typedef struct Listnode
    {

        char* same_item_string;                 //ena stng me ta koina items metaksu istotopwn (orizontia)
        struct node *next;
    }Llist;
    typedef struct content_table
    {
        Llist  *list_ptr;
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
    int count,i = 0;
    struct dirent *de;  // pointer sto directory


    DIR *dr = opendir("C:\\Users\\Yannis\\Datasets\\");      // opendir() =epistrefei pointer tou directory.

    if (dr == NULL)  //
    {
        printf("Could not open current directory" );
        return 0;
    }

    while ((de = readdir(dr)) != NULL)             //mporoume na paiksoume me readdir?!!!! diavasma arxeiwn katalogou gia malloc array
        count++;

    closedir(dr);
    Llist * head = NULL;
    head = (Llist *) malloc(sizeof(Llist));
    if (head == NULL) {
        return 1;
    }

    content_table array_contents[count] ;
    for (i=0;i>=count;i++)
    {
        array_contents[i].list_ptr = NULL;
        array_contents[i].list_ptr = (Llist *) malloc(sizeof(Llist));
        /*if (array_contents[i].list_ptr == NULL) {
            return 1;
        }*/
        array_contents[i].item_address=NULL;
        array_contents[i].potition=0;

    }
    //Llist * array;   //pinakas apo deiktes pou deixnoun se llist nodes
    //array = malloc(count * sizeof(Llist*));

    return 0;

}
