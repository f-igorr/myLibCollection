#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "myDict.h"
#include "funcs.h"


/*
Словарь
ключи: только строки
данные: TypedVal (TYPE_NONE, TYPE_INT, TYPE_FLOAT, TYPE_DOUBLE, TYPE_STRING)
*/


typedef struct dNode {
    char     *key;
    TypedVal *tval;
    struct dNode* next;
} dNode;

typedef struct Dict {
    dNode** buckets;
    size_t size;
    size_t count;
} Dict;

//========================================


static dNode* _Create_Node ()
{
    dNode* new = (dNode*) calloc (1, sizeof (dNode));
    assert (new);
    new->tval = Create_typed_value ();
    return new;
}

static void _Resize (Dict* dict, const size_t new_size)
{
    assert (dict != NULL);
    assert (new_size > dict->size);

    // если использовать malloc то надо все элементы иниц NULL (или 0)
    dNode ** new_buckets = (dNode**) calloc (new_size , sizeof (dNode*));
    assert (new_buckets);

    dNode* curr, *tmp;
    size_t new_ind;
    size_t old_size = dict->size;
    // проходим по старым корзинам
    for (size_t old_ind = 0; old_ind < old_size; old_ind++) {
        curr = dict->buckets[old_ind];
        // проходим по цепочке
        while (curr) {
            tmp = curr->next;
            new_ind = djb2_hash_str (curr->key) % new_size;
            // вставка элемента в голову цепочки
            curr->next = new_buckets[new_ind];
            new_buckets[new_ind] = curr;
            curr = tmp;
        }
    }
    free (dict->buckets);
    dict->size = new_size;
    dict->buckets = new_buckets;
}

//==========================================================

Dict* Dict_Create (size_t size)
{
    //size = size < DICT_DEFAULT_SIZE ? DICT_DEFAULT_SIZE : size;

    Dict* dict = (Dict*) calloc (1, sizeof (Dict));
    assert (dict);
    
    dict->size = size;
    dict->count = 0;

    // если использовать malloc то надо все элементы иниц NULL (или 0)
    dict->buckets = (dNode**) calloc (size , sizeof (dNode*));
    assert (dict->buckets);

    return dict;
}

void Dict_Destroy (Dict** ppdict)
{
    assert (ppdict != NULL);
    Dict *dict = *ppdict;
    assert (dict != NULL);

    dNode *curr, *next;
    for (size_t i = 0; i < dict->size; i++) {
        curr = dict->buckets[i];
        while (curr) {
            next = curr->next;
            free (curr->key);
            Destroy_typed_value (&(curr->tval));
            free (curr);
            curr = next;
        }
    }
    free (dict->buckets);
    free (dict);
    *ppdict = NULL;
}


STATUS_PUT Dict_Put (Dict* dict, const char* key, const TypedVal *tval)
{
    assert (dict != NULL);
    assert (key  != NULL);
    assert (tval != NULL);

    // check for resize
    if ( (float) (dict->count + 1) / (float) dict->size > DICT_LOAD_FACTOR) {
        _Resize(dict, dict->size * DICT_RESIZE_FACTOR);
    }
    
    size_t ind = djb2_hash_str (key) % dict->size;
    dNode* curr = dict->buckets[ind];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            //curr->val = val; // так нельзя, надо копировать
            // free old s_val in copy_typed_value()
            Copy_typed_value (curr->tval, tval);
            return UPDATED;
        }
        curr = curr->next;
    }
    curr = _Create_Node ();
    curr->key = my_strdup(key);
    //curr->val = val; // так нельзя, надо копировать
    Copy_typed_value (curr->tval, tval);
    curr->next = dict->buckets[ind];
    dict->buckets[ind] = curr;
    dict->count++;
    return INSERTED;
}


bool Dict_Get (const Dict* dict, const char* key, TypedVal *res)
{
    // !!!  СТАРОЕ ЗНАЧЕНИЕ RES БУДЕТ УДАЛЕНО !!!
    // return: bool (found or not)
    
    assert (dict != NULL);
    assert (key != NULL);
    assert (res != NULL);

    size_t ind  = djb2_hash_str (key) % dict->size;
    dNode* curr  = dict->buckets[ind];
    while (curr) {
        if (strcmp (curr->key, key) == 0) {
            //*res = curr->val; // так нельзя, надо копировать
            Copy_typed_value (res, curr->tval);
            return true;
        }
        curr = curr->next;
    }
    return false;
}


bool Dict_Remove (Dict* dict, const char* key)
{
    // удаление по ключу (ключ только STRING)
    // return: true (removed) OR false (not found)

    assert (dict !=NULL);
    assert (key != NULL);

    size_t ind = djb2_hash_str (key) % dict->size;
    dNode *curr = dict->buckets[ind];
    dNode *prev = NULL;

    while (curr) {
        if (strcmp (curr->key, key) == 0) {
            // изменяем ссылки
            if (prev)   prev->next = curr->next;
            else        dict->buckets[ind] = curr->next;
            // удаляем ноду
            free (curr->key);
            Destroy_typed_value (&(curr->tval));
            free (curr);
            dict->count--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}


bool Dict_Contains (Dict* dict, const char* key)
{
    // return: bool (found or not)
    
    assert (dict != NULL);
    assert (key != NULL);

    size_t ind  = djb2_hash_str (key) % dict->size;
    dNode* curr  = dict->buckets[ind];
    while (curr) {
        if (strcmp (curr->key, key) == 0) {
            return true; // found
        }
        curr = curr->next;
    }
    return false; // not found
}


size_t Dict_Len (Dict *dict)
{
    return dict->count;
}


void Dict_Print (const Dict* dict)
{
    assert (dict != NULL);

    printf ("\ndict: size = %ld, count = %ld, (key,val): \n", dict->size, dict->count);
    if (dict->count == 0) {
        printf ("Dict empty\n");
        return;
    }
    dNode *curr;
    for (size_t i = 0; i < dict->size; i++) {
        curr = dict->buckets[i];
        if (curr) printf ("bucket [%ld]: ", i);
        while (curr) {
            printf ("(%s,", curr->key);
            typed_value_print (curr->tval);
            if (curr->next) printf ("-->");
            curr = curr->next;
            if (!curr) printf ("\n");
        }
    }
}