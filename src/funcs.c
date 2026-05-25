#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "funcs.h"


size_t djb2_hash_str (const char *s) 
{
    assert (s != NULL);

    size_t hash = 5381;
    unsigned int c;
    while ((c = *s++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}


char* my_strdup (const char* src)
{
    if (src == NULL) return NULL;

    char* dst = (char*) calloc (strlen(src) + 1, sizeof (char));
    assert (dst);

    strcpy (dst, src);
    return dst;
}

