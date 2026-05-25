#ifndef _MY_DICT_H_
#define _MY_DICT_H_


#include "typed_value.h"


//#define DICT_DEFAULT_SIZE 128
#define DICT_LOAD_FACTOR 1.0
#define DICT_RESIZE_FACTOR 2


typedef enum {
    INSERTED,
    UPDATED
} STATUS_PUT;


typedef struct Dict Dict;


Dict* Dict_Create (size_t size);
void Dict_Destroy (Dict** dict);
STATUS_PUT Dict_Put (Dict* dict, const char* key, const TypedVal *val);
bool Dict_Get (const Dict* dict, const char* key, TypedVal *res);
bool Dict_Remove (Dict* dict, const char* key);
bool Dict_Contains (Dict* dict, const char* key);
size_t Dict_Len (Dict *dict);
void Dict_Print (const Dict* dict);



#endif