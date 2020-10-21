#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dirent.h>

int main(void) {
    typedef struct Listnode
    {

        char* sameitemstring;
        struct node *next;
    }llist;
    typedef struct array
    {

        char* itemaddress;
        int potition;
    }contenttable;

    //// gia tuxon data
    /*llist **new_array;
    new_array = realloc(array, new_size * sizeof(LList*));
    */
///file  count lines mallon de xreiazetai
    /*FILE *fp;

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

    int count = 0;
    struct dirent *de;  // pointer sto directory

    // opendir() =epistrefei pointer tou directory.
    DIR *dr = opendir("C:\\Users\\Yannis\\Datasets\\");

    if (dr == NULL)  //
    {
        printf("Could not open current directory" );
        return 0;
    }
    //mporoume na paiksoume me readdir?!!!!
    while ((de = readdir(dr)) != NULL)
        count++;

    closedir(dr);

    llist ** array;
    array = malloc(count * sizeof(llist*));

    return 0;

}
