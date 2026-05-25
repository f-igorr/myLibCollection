#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "myDict.h"
#include "funcs.h"


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


void test_Dict_Create ()
{
    size_t size = 10;
    Dict *dict = Dict_Create (size);

    assert (dict->count == 0);
    assert (dict->size == size);
    for (size_t i = 0; i < size; i++) 
        assert (dict->buckets[i] == NULL);
    Dict_Destroy (&dict);
    printf ("[test_Dict_Create] OK\n");
}


void test__Resize ()
{
    size_t size = 1;
    Dict *dict = Dict_Create (size);
    TypedVal *tv = Create_typed_value ();
    void* data = calloc (100, 8);
    char buf [100] = {0};
    
    size_t count_resize = 0, iter = 32, tmp;
    size_t corr_count_resize = 0;
    tmp = iter;
    while (tmp >= 2) {
        tmp /= 2;
        corr_count_resize++;
    }

    for (size_t i = 0; i < iter; i++) {
        tmp = dict->size;
        *(int*) data = (int) i * 10;
        Set_typed_value (data, TYPE_INT, tv);
        sprintf (buf, "%ld", i + 5);
        Dict_Put (dict, buf, tv);
        if (dict->size != tmp)
            count_resize++;
    }
    assert (dict->count == iter);
    assert (corr_count_resize == count_resize);

    Dict_Destroy (&dict);
    Destroy_typed_value (&tv);
    free (data);
    printf ("[test__Resize] OK\n");
}


void test_Dict_Destroy ()
{
    TypedVal *tv = Create_typed_value ();
    void* data = calloc (100, 8);
    Dict *dict = NULL;
    char *key = "hello";

    dict = Dict_Create (10);
    *(int*) data = 123;
    Set_typed_value (data, TYPE_INT, tv);
    Dict_Put (dict, key, tv);
    assert (Dict_Contains (dict, key));
    Dict_Destroy (&dict);
    assert (dict == NULL);

    dict = Dict_Create (10);
    strcpy ((char*) data , "hello");
    Set_typed_value (data, TYPE_STRING, tv);
    Dict_Put (dict, key, tv);
    assert (Dict_Contains (dict, key));
    Dict_Destroy (&dict);
    assert (dict == NULL);

    Destroy_typed_value (&tv);
    free (data);
    printf ("[test_Dict_Destroy] OK\n");
}


static bool check_unique_data (const size_t *arr, size_t indx)
{
    if (indx == 0) return true;
    size_t data = arr[indx];
    while (indx) {
        indx--;
        if (arr[indx] == data) return false;
    }
    return true;
}


void test_Dict_Put_and_Contains ()
{
    const size_t all = 4;

    TypedVal *tv = Create_typed_value ();
    void* data = calloc (100, 8);
    Dict *dict = Dict_Create (1000);
    size_t indx[all], hash, i;
    size_t count = 0;
    char key[1000] = {0};

    i = 0;
    *(int*) data = 123;
    sprintf (key, "%d", *(int*)data);
    Set_typed_value (data, TYPE_INT, tv);
    if (Dict_Put (dict, key, tv) == INSERTED)
        count++;
    assert (Dict_Put (dict, key, tv) == UPDATED);
    assert (dict->count == count && count == i+1);
    hash = djb2_hash_str (key);
    indx[i] = hash % dict->size;
    assert (check_unique_data (indx, i)); // если индексы повторяются то нужно проходить по цепочкам
    assert (Equal_typed_value (dict->buckets[indx[i]]->tval, tv));

    i = 1;
    *(float*) data = 456.0f;
    sprintf (key, "%f", *(float*)data);
    Set_typed_value (data, TYPE_FLOAT, tv);
    if (Dict_Put (dict, key, tv) == INSERTED)
        count++;
    assert (Dict_Put (dict, key, tv) == UPDATED);
    assert (dict->count == count && count == i+1);
    hash = djb2_hash_str (key);
    indx[i] = hash % dict->size;
    assert (check_unique_data (indx, i)); // если индексы повторяются то нужно проходить по цепочкам
    assert (Equal_typed_value (dict->buckets[indx[i]]->tval, tv));

    i = 2;
    *(double*) data = 789.0;
    sprintf (key, "%lf", *(double*)data);
    Set_typed_value (data, TYPE_DOUBLE, tv);
    if (Dict_Put (dict, key, tv) == INSERTED)
        count++;
    assert (Dict_Put (dict, key, tv) == UPDATED);
    assert (dict->count == count && count == i+1);
    hash = djb2_hash_str (key);
    indx[i] = hash % dict->size;
    assert (check_unique_data (indx, i)); // если индексы повторяются то нужно проходить по цепочкам
    assert (Equal_typed_value (dict->buckets[indx[i]]->tval, tv));

    i = 3;
    strcpy ((char*) data , "hello");
    sprintf (key, "%s", (char*)data);
    Set_typed_value (data, TYPE_STRING, tv);
    if (Dict_Put (dict, key, tv) == INSERTED)
        count++;
    assert (Dict_Put (dict, key, tv) == UPDATED);
    assert (dict->count == count && count == i+1);
    hash = djb2_hash_str (key);
    indx[i] = hash % dict->size;
    assert (check_unique_data (indx, i)); // если индексы повторяются то нужно проходить по цепочкам
    assert (Equal_typed_value (dict->buckets[indx[i]]->tval, tv));

    Dict_Destroy (&dict);
    Destroy_typed_value (&tv);
    free (data);
    printf ("[test_Dict_Put_and_Contains] OK\n");
}


void test_Dict_Get ()
{
    TypedVal *tv  = Create_typed_value ();
    TypedVal *res = Create_typed_value ();
    void* data = calloc (100, 8);
    Dict *dict = Dict_Create (10);
    char key[1000] = {0};

    *(int*) data = 123;
    sprintf (key, "%d", *(int*)data);
    Set_typed_value (data, TYPE_INT, tv);
    Dict_Put (dict, key, tv);
    Dict_Get (dict, key, res);
    assert (Equal_typed_value (res, tv));
    _Clean_typed_value (res);

    *(float*) data = 321.0f;
    sprintf (key, "%f", *(float*)data);
    Set_typed_value (data, TYPE_FLOAT, tv);
    Dict_Put (dict, key, tv);
    Dict_Get (dict, key, res);
    assert (Equal_typed_value (res, tv));
    _Clean_typed_value (res);

    *(double*) data = 456.0;
    sprintf (key, "%fl", *(double*)data);
    Set_typed_value (data, TYPE_DOUBLE, tv);
    Dict_Put (dict, key, tv);
    Dict_Get (dict, key, res);
    assert (Equal_typed_value (res, tv));
    _Clean_typed_value (res);

    strcpy ((char*) data , "hello");
    sprintf (key, "%s", (char*)data);
    Set_typed_value (data, TYPE_STRING, tv);
    Dict_Put (dict, key, tv);
    Dict_Get (dict, key, res);
    assert (Equal_typed_value (res, tv));
    //_Clean_typed_value (res);

    Dict_Destroy (&dict);
    Destroy_typed_value (&tv);
    Destroy_typed_value (&res);
    free (data);
    printf ("[test_Dict_Get] OK\n");
}


void test_Dict_Remove ()
{
    Dict *dict = Dict_Create (16);
    TypedVal *tv = Create_typed_value ();
    void* data = calloc (100, 8);
    char key[1000] = {0};

    *(int*) data = 123;
    sprintf (key, "%d", *(int*)data);
    Set_typed_value (data, TYPE_INT, tv);
    Dict_Put (dict, key, tv);
    Dict_Remove (dict, key);
    assert (Dict_Contains (dict, key) == false);
    assert (dict->count == 0);
    Dict_Remove (dict, key);

    *(float*) data = 456.0f;
    sprintf (key, "%f", *(float*)data);
    Set_typed_value (data, TYPE_FLOAT, tv);
    Dict_Put (dict, key, tv);
    Dict_Remove (dict, key);
    assert (Dict_Contains (dict, key) == false);
    assert (dict->count == 0);
    Dict_Remove (dict, key);

    *(double*) data = 456.0f;
    sprintf (key, "%lf", *(double*)data);
    Set_typed_value (data, TYPE_DOUBLE, tv);
    Dict_Put (dict, key, tv);
    Dict_Remove (dict, key);
    assert (Dict_Contains (dict, key) == false);
    assert (dict->count == 0);
    Dict_Remove (dict, key);

    strcpy ((char*) data , "hello");
    sprintf (key, "%s", (char*)data);
    Set_typed_value (data, TYPE_STRING, tv);
    Dict_Put (dict, key, tv);
    Dict_Remove (dict, key);
    assert (Dict_Contains (dict, key) == false);
    assert (dict->count == 0);
    Dict_Remove (dict, key);

    Dict_Destroy (&dict);
    Destroy_typed_value (&tv);
    free (data);
    printf ("[test_Dict_Remove] OK\n");

}

void test_Dict_Len ()
{
    Dict *dict = Dict_Create (16);
    TypedVal *tv = Create_typed_value ();
    void* data = calloc (100, 8);
    char key[1000] = {0};

    size_t iters = 10;
    for (size_t i = 0; i < iters; i++) {
        *(int*) data = (int) i * 2;
        sprintf (key, "%d", *(int*)data);
        Set_typed_value (data, TYPE_INT, tv);
        Dict_Put (dict, key, tv);
        assert (Dict_Len (dict) == i+1);
    }
    assert (dict->count == iters);
    assert (Dict_Len (dict) == iters);
    
    Dict_Destroy (&dict);
    Destroy_typed_value (&tv);
    free (data);
    printf ("[test_Dict_Len] OK\n");
}



int main()
{
    test_Dict_Create ();
    test__Resize ();
    test_Dict_Destroy ();
    test_Dict_Put_and_Contains ();
    test_Dict_Get ();
    test_Dict_Remove ();
    test_Dict_Len ();

    return 0;
}