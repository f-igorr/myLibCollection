#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


#include "myHashSet.h"


typedef struct hsNode {
    TypedVal      *tval;
    struct hsNode *next;
} hsNode;

typedef struct HashSet {
    hsNode** buckets;
    size_t size;
    size_t count;
} HashSet;



static hsNode* _Create_Node ()
{
    hsNode* new = (hsNode*) calloc (1, sizeof (hsNode));
    assert (new);
    new->tval = Create_typed_value ();
    return new;
}

static void _Resize (HashSet* hashset, const size_t new_size)
{
    assert (hashset != NULL);
    assert (new_size > hashset->size);

    // если использовать malloc то надо все элементы иниц NULL (или 0)
    hsNode ** new_buckets = (hsNode**) calloc (new_size , sizeof (hsNode*));
    assert (new_buckets);

    hsNode* curr, *tmp;
    size_t new_ind;
    size_t old_size = hashset->size;
    // проходим по старым корзинам
    for (size_t old_ind = 0; old_ind < old_size; old_ind++) {
        curr = hashset->buckets[old_ind];
        // проходим по цепочке
        while (curr) {
            tmp = curr->next;
            new_ind = Hash_typed_value (curr->tval) % new_size;
            // вставка элемента в голову цепочки
            curr->next = new_buckets[new_ind];
            new_buckets[new_ind] = curr;
            curr = tmp;
        }
    }
    free (hashset->buckets);
    hashset->size = new_size;
    hashset->buckets = new_buckets;
    //printf ("\nRESIZE: %ld -> %ld\n", old_size, new_size);
}



//==========================================================

HashSet* HashSet_Create (size_t size)
{
    //size = size < HASHSET_DEFAULT_SIZE ? HASHSET_DEFAULT_SIZE : size;

    HashSet* hashset = (HashSet*) calloc (1, sizeof (HashSet));
    assert (hashset);
    
    hashset->size = size;
    hashset->count = 0;

    // если использовать malloc то надо все элементы иниц NULL (или 0)
    hashset->buckets = (hsNode**) calloc (size , sizeof (hsNode*));
    assert (hashset->buckets);

    return hashset;
}

void HashSet_Destroy (HashSet** pphashset)
{
    assert (pphashset != NULL);
    HashSet *hashset = *pphashset;
    assert (hashset != NULL);

    hsNode *curr, *next;
    for (size_t i = 0; i < hashset->size; i++) {
        curr = hashset->buckets[i];
        while (curr) {
            next = curr->next;
            Destroy_typed_value (&(curr->tval));
            free (curr);
            curr = next;
        }
    }
    free (hashset->buckets);
    free (hashset);
    *pphashset = NULL;
}


bool HashSet_Contains (const HashSet* hashset, const TypedVal *tval)
{
    // return: bool (found or not)
    
    assert (hashset != NULL);

    size_t ind  = Hash_typed_value (tval) % hashset->size;
    hsNode* curr  = hashset->buckets[ind];
    while (curr) {
        if (Equal_typed_value (curr->tval, tval)) {
            return true; // found
        }
        curr = curr->next;
    }
    return false; // not found
}


bool HashSet_Add (HashSet* hashset, const TypedVal *tval)
{
    assert (hashset != NULL);
    assert (tval != NULL);

    if (HashSet_Contains (hashset, tval)) 
        return false; // уже есть

    // check for resize
    if ( (float) (hashset->count + 1) / (float) hashset->size > HASHSET_LOAD_FACTOR) {
        _Resize(hashset, hashset->size * HASHSET_RESIZE_FACTOR);
    }
    
    hsNode* new = _Create_Node();
    Copy_typed_value (new->tval, tval); // нужно глубокое копирование с выделением пямяти
    
    size_t ind = Hash_typed_value (tval) % hashset->size;
    new->next = hashset->buckets[ind];
    hashset->buckets[ind] = new;
    hashset->count++;

    return true;
}


bool HashSet_Remove (HashSet* hashset, const TypedVal *tval)
{
    // return: true (removed) OR false (not found)

    assert (hashset !=NULL);

    size_t ind = Hash_typed_value (tval) % hashset->size;
    hsNode *curr = hashset->buckets[ind];
    hsNode* prev = NULL;

    while (curr) {
        if (Equal_typed_value (curr->tval, tval)) {
            // изменяем ссылки
            if (prev) prev->next = curr->next;
            else      hashset->buckets[ind] = curr->next;
            // удаляем ноду
            Destroy_typed_value (&(curr->tval));
            free (curr);
            hashset->count--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}


/* длина сета (кол элементов)
    принимает указ 
    возвращает кол-во элементов (длина)
*/
size_t HashSet_Len (HashSet *hashset)
{
    return hashset->count;
}


void HashSet_Print (const HashSet* hashset)
{
    printf ("\nHashSet: size = %ld, count = %ld\n", hashset->size, hashset->count);
    if (hashset->count == 0) {
        printf ("Hashset empty\n");
        return;
    }
    hsNode *curr;
    for (size_t i = 0; i < hashset->size; i++) {
        curr = hashset->buckets[i];
        if (curr) 
            printf ("bucket [%ld]: ", i);
        while (curr) {
            typed_value_print (curr->tval);
            if (curr->next) printf ("-->");
            curr = curr->next;
            if (!curr) printf ("\n");
        }
    }
}