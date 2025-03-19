#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "netinfo.h"
int GetNetInfoIpFunc(void **data, unsigned int *len)
{
    char        *tmp    = "127.0.0.1";
    unsigned int tmpLen = strlen(tmp) + 1;
    *len                = tmpLen - 1;
    *data = malloc(tmpLen);
    if (*data == NULL) {
        printf("malloc err\r\n");
        return -1;
    }
    (void)memset(*data, 0, tmpLen);
    (void)strcpy(*data, tmp);
    return 0;
}