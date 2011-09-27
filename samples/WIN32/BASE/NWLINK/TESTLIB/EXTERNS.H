#include <windows.h>
#include <wsipx.h>
#include "testlib.h"

typedef struct tag_error_struct{
    int     errnum;
    LPSTR   errstr;
} ERROR_STRUCT;


LPSTR get_network_number(LPSTR);
LPSTR get_node_number(LPSTR);
LPSTR get_socket_number(LPSTR);

char msg_no_error[];
char msg_unknown_error[];

