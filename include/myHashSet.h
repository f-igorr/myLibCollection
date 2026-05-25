#ifndef _MY_HASHSET_H_
#define _MY_HASHSET_H_

#include "typed_value.h"


// PARAMs
//#define HASHSET_DEFAULT_SIZE 128
#define HASHSET_LOAD_FACTOR 1.0
#define HASHSET_RESIZE_FACTOR 2


typedef struct HashSet HashSet;


HashSet* HashSet_Create (size_t size);
void HashSet_Destroy (HashSet** hashset);
bool HashSet_Contains (const HashSet* hashset, const TypedVal *val);
bool HashSet_Add (HashSet* hashset, const TypedVal *val);
bool HashSet_Remove (HashSet* hashset, const TypedVal *val);
void HashSet_Print (const HashSet* hashset);
size_t HashSet_Len (HashSet *hashset);


#endif //_MY_HASHSET_H_