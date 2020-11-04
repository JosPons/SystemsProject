
#ifndef SYSTEMSPROJECT_DEFINITIONS_H
#define SYSTEMSPROJECT_DEFINITIONS_H




#include <malloc.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>



typedef struct listnode {

    char *json_id;                  //ebayjson22

    struct listnode *next;
    struct listnode *previous;
} list_node;

typedef struct contenttable {
    list_node *list_ptr;
    char *site_address;    //e.g amazon /ebay /ktl
    char *spec_id;  //json222
} content_table;


#endif //SYSTEMSPROJECT_DEFINITIONS_H