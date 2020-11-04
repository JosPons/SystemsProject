#include <stdio.h>

#include "definitions.h"

int main(void) {


    struct dirent *de;  // pointer sto directory

    char *filename;

    printf("Enter FILE PATH: ");
    //scanf("%s", filename);

    filename="C:\\Users\\g.kabardis\\Desktop\\camera_specs\\2013_camera_specs\\";
    printf("%s \n ", filename);

    DIR *dr = opendir(filename);     //epistrefei pointer tou directory.
    if (dr == NULL)  //
    {
        printf("Could not open current directory");
        return 0;
    }
    int count = 0;
    while ((de = readdir(dr)) !=
           NULL) {
        count++;   //sunolo arxeiwn directory  (ebay,amazon,ebet ktl)
    }
    //closedir(dr);
    printf(" arithmos directories = %d \n", count - 2);
    ///////////////////////////////////////////////////////////////////////////
    rewinddir(dr);
    char *dir_array[count - 2];

    int json_num[count - 2];        //pinakas me arithmos json ana directory
    int i = 0;
    while ((de = readdir(dr)) !=
           NULL) {
        dir_array[i] = malloc((strlen(de->d_name) + 1) * sizeof(char));
        strcpy(dir_array[i], de->d_name);       //array me directories selidwn
        //printf("%s\n",dir_array[i]);
        i++;
    }

    closedir(dr);
    //strcpy(filename, temp);
    //rewinddir(dr);
    ////////////////////////////////////////////////////
//"C:\\Users\\yannis\\Desktop\\camera_specs\\2013_camera_specs\\"
    // char *dir= malloc(((strlen(filename) +1)*2)* sizeof(char ));  //mporei na ginei auto gt malloc se kathe loop, isws einai xeirotero logw pollwn malloc

    int count2 = 0;
    for (i = 0; i <= count - 2; i++) {
        char *dir = malloc((strlen(filename) + strlen(dir_array[i]) + 2) * sizeof(char));
        strcpy(dir, filename);
        strcat(dir, dir_array[i]);
        count2 = 0;
        //printf(dir);
        dr = opendir(dir);
        if (dr == NULL) {
            printf(dir);
            printf("Could not open current directory\n");
            return 0;
        }

        while ((de = readdir(dr)) !=
               NULL) {
            count2++;       // arithmos json
            // printf("  %s \n", de->d_name ); //DIR NAME
        }
        json_num[i] = count2 - 2;

        printf("arithmos json  gia kathe directory %d\n", json_num[i]); //DIR NAME
        free(dir);

    }
    //printf("rdy\n");
    // closedir(dr);
///////////////////////////////////////////
    int total = 0;
    for (i = 0; i <= count - 2; i++) {
        total = total + json_num[i];

    }
    printf("sunolikos arithmos json,ara kai megethos domhs  %d\n", total);

    list_node *head = NULL;
    head = (list_node *) malloc(sizeof(list_node));
    if (head == NULL) {
        return 1;
    }

    content_table array_contents[total];
    for (i = 0; i <= count - 2; i++) {
        array_contents[i].list_ptr = NULL;
        array_contents[i].list_ptr = (list_node *) malloc(sizeof(list_node));

        array_contents[i].spec_id = NULL;
        array_contents[i].site_address = NULL;

    }

    printf(" domh arxikopoihsh done \n");

    //rewinddir(dr);
    ////////////////////////////////////////////////////
    for (i = 0; i <= count - 2; i++) {
        char *dir = malloc((strlen(filename) + strlen(dir_array[i]) + 2) * sizeof(char));
        strcpy(dir, filename);
        strcat(dir, dir_array[i]);

        dr = opendir(dir);//
        if (dr == NULL)  //
        {
            printf("Could not open current directory\n");
            return 0;
        }

        while ((de = readdir(dr)) !=
               NULL) {
            array_contents[i].spec_id = malloc((strlen(de->d_name) + 1) * sizeof(char));
            strcpy(array_contents[i].spec_id, de->d_name);
            array_contents[i].site_address = malloc((strlen( dir_array[i]) + 1) * sizeof(char));
            strcpy(array_contents[i].site_address, dir_array[i]);
            //array_contents[i].list_ptr->json_id=malloc((strlen( dir_array[i]) +strlen( de->d_name) + 1) * sizeof(char));
            //strcpy(array_contents[i].list_ptr->json_id, de->d_name);   // a=a , b=b ktl
            i++;
        }
        //countdif[i] = count2-2;
        // printf("%d\n", countdif[i]); //DIR NAME
        free(dir);
    }

    printf("dhmiourgia domhs kai eisagwgh data done \n");

    FILE *stream = fopen("C:\\Users\\g.kabardis\\Desktop\\sigmod_medium_labelled_dataset.csv",
                         "r"); //diavasma excel gia dhmiourgia LISTWN

    char line[1024];
    while (fgets(line, 1024, stream)) {
        char *tmp = strdup(line);
        char *tok;
        for (tok = strtok(tmp, ",");
             tok && *tok;
             tok = strtok(NULL, "")) {
            if (tok != NULL && tok != "0") {
                // epistrefei ola ta tokens me th seira.to 3o einai to 1 h 0
                //      edw tha trexei o algorithmos ths listas
                //  printf("diavasma excel file %s \n",tok);
            }

        }
        free(tmp);
    }
    printf("Field 3 is 1.change pointers between array and lists  \n");

    free(stream);

    return 0;

}
/*      PROFANWS THELOUN ORGANWSH SE HEADERS,.C KTL.
      MEXRI TWRA DIAVASAME DIRECTORIES GIA NA VROUME POSA EINAI SUNOLO (EBAY/AMAZON/KTL )KAI
       FTIAXNOUME ENA PINAKA APO STRUCTS DEIKTWN SE LINKED LISTS ME PLHTHOS AUTO ,TWN DIRECTORIES.
      KATHE DIRECTORY ADISTOIXEI SE MIA THESH STON ARRAY ,OPOTE KSANADIAVASAME GIA NA VALOUME SE KATHE THESH TOU ARRAY NAME APO PROION
       KAI ADISTOIXIA ME TON EUATO TOU.
       META DIAVASAME TO EXCEL KAI THA FTIAXTOUN
      OI KOMVOI THS LISTAS KAI ANALOGA ME TO AN YPARXEI 1 STO 3o PEDIO KATHE GRAMMHS TOU EXCELL .EDW PAIZEITAI OLO TO PRAGMA
      ME TO PWS THA KANOUME TIS SUGKRISEIS APODOTIKA NA MHN KSANAGURNAME PISW SUNEXEIA .EPISHS ISWS H PRWTH DOMH STRUCT ARRAY ISWS PREPEI NA GINEI DENTRO*/
