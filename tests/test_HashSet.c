#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "myHashSet.h"


typedef struct hsNode {
    TypedVal  *tval;
    struct hsNode* next;
} hsNode;

typedef struct HashSet {
    hsNode** buckets;
    size_t size;
    size_t count;
} HashSet;

//========================================


void test_HashSet_Create ()
{
    size_t size = 10;
    HashSet *hashset = HashSet_Create (size);

    assert (hashset->count == 0);
    assert (hashset->size == size);
    for (size_t i = 0; i < size; i++) 
        assert (hashset->buckets[i] == NULL);
    HashSet_Destroy (&hashset);
    printf ("[test_HashSet_Create] OK\n");
}


void test__Resize ()
{
    size_t size = 1;
    HashSet *hashset = HashSet_Create (size);
    TypedVal *tv = Create_typed_value ();
    void* data = calloc (100, 8);
    
    size_t count_resize = 0, iter = 32, tmp;
    size_t corr_count_resize = 0;
    tmp = iter;
    while (tmp >= 2) {
        tmp /= 2;
        corr_count_resize++;
    }

    for (size_t i = 0; i < iter; i++) {
        tmp = hashset->size;
        *(int*) data = (int) i * 10;
        Set_typed_value (data, TYPE_INT, tv);
        HashSet_Add (hashset, tv);
        if (hashset->size != tmp)
            count_resize++;
    }
    assert (hashset->count == iter);
    assert (corr_count_resize == count_resize);

    HashSet_Destroy (&hashset);
    Destroy_typed_value (&tv);
    free (data);
    printf ("[test__Resize] OK\n");
}


void test_HashSet_Destroy ()
{
    TypedVal *tv = Create_typed_value ();
    void* data = calloc (100, 8);
    HashSet *hashset = NULL;

    hashset = HashSet_Create (10);
    *(int*) data = 123;
    Set_typed_value (data, TYPE_INT, tv);
    HashSet_Add (hashset, tv);
    assert (HashSet_Contains (hashset, tv));
    HashSet_Destroy (&hashset);
    assert (hashset == NULL);


    hashset = HashSet_Create (10);
    strcpy ((char*) data , "hello");
    Set_typed_value (data, TYPE_STRING, tv);
    HashSet_Add (hashset, tv);
    assert (HashSet_Contains (hashset, tv));
    HashSet_Destroy (&hashset);
    assert (hashset == NULL);

    Destroy_typed_value (&tv);
    free (data);
    printf ("[test_HashSet_Destroy] OK\n");
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


void test_HashSet_Add_and_Contains ()
{
    TypedVal *tv = Create_typed_value ();
    void* data = calloc (100, 8);
    HashSet *hashset = HashSet_Create (1000);
    size_t indx[4] = {0}, hash, i;
    size_t count = 0;

    i = 0;
    *(int*) data = 123;
    Set_typed_value (data, TYPE_INT, tv);
    if (HashSet_Add (hashset, tv))
        count++;
    if (HashSet_Add (hashset, tv))
        count++;
    assert (hashset->count == count && count == i+1);
    hash = Hash_typed_value (tv);
    indx[i] = hash % hashset->size;
    assert (check_unique_data (indx, i)); // если индексы повторяются то нужно проходить по цепочкам
    assert (Equal_typed_value (hashset->buckets[indx[i]]->tval, tv));

    i = 1;
    *(float*) data = 456.0f;
    Set_typed_value (data, TYPE_FLOAT, tv);
    if (HashSet_Add (hashset, tv))
        count++;
    if (HashSet_Add (hashset, tv))
        count++;
    assert (hashset->count == count && count == i+1);
    hash = Hash_typed_value (tv);
    indx[i] = hash % hashset->size;
    assert (check_unique_data (indx, i)); // если индексы повторяются то нужно проходить по цепочкам
    assert (Equal_typed_value (hashset->buckets[indx[i]]->tval, tv));

    i = 2;
    *(double*) data = 789.0;
    Set_typed_value (data, TYPE_DOUBLE, tv);
    if (HashSet_Add (hashset, tv))
        count++;
    if (HashSet_Add (hashset, tv))
        count++;
    assert (hashset->count == count && count == i+1);
    hash = Hash_typed_value (tv);
    indx[i] = hash % hashset->size;
    assert (check_unique_data (indx, i)); // если индексы повторяются то нужно проходить по цепочкам
    assert (Equal_typed_value (hashset->buckets[indx[i]]->tval, tv));

    i = 3;
    strcpy ((char*) data , "hello");
    Set_typed_value (data, TYPE_STRING, tv);
    if (HashSet_Add (hashset, tv))
        count++;
    if (HashSet_Add (hashset, tv))
        count++;
    assert (hashset->count == count && count == i+1);
    hash = Hash_typed_value (tv);
    indx[i] = hash % hashset->size;
    assert (check_unique_data (indx, i)); // если индексы повторяются то нужно проходить по цепочкам
    assert (Equal_typed_value (hashset->buckets[indx[i]]->tval, tv));

    HashSet_Destroy (&hashset);
    Destroy_typed_value (&tv);
    free (data);
    printf ("[test_HashSet_Add_and_Contains] OK\n");
}


void test_HashSet_Remove ()
{
    HashSet *hashset = HashSet_Create (16);
    TypedVal *tv = Create_typed_value ();
    void* data = calloc (100, 8);

    *(int*) data = 123;
    Set_typed_value (data, TYPE_INT, tv);
    HashSet_Add (hashset, tv);
    HashSet_Remove (hashset, tv);
    assert (HashSet_Contains (hashset, tv) == false);
    assert (hashset->count == 0);
    HashSet_Remove (hashset, tv);

    *(float*) data = 456.0f;
    Set_typed_value (data, TYPE_FLOAT, tv);
    HashSet_Add (hashset, tv);
    HashSet_Remove (hashset, tv);
    assert (HashSet_Contains (hashset, tv) == false);
    assert (hashset->count == 0);
    HashSet_Remove (hashset, tv);

    *(double*) data = 456.0f;
    Set_typed_value (data, TYPE_DOUBLE, tv);
    HashSet_Add (hashset, tv);
    HashSet_Remove (hashset, tv);
    assert (HashSet_Contains (hashset, tv) == false);
    assert (hashset->count == 0);
    HashSet_Remove (hashset, tv);

    strcpy ((char*) data , "hello");
    Set_typed_value (data, TYPE_STRING, tv);
    HashSet_Add (hashset, tv);
    HashSet_Remove (hashset, tv);
    assert (HashSet_Contains (hashset, tv) == false);
    assert (hashset->count == 0);
    HashSet_Remove (hashset, tv);

    HashSet_Destroy (&hashset);
    Destroy_typed_value (&tv);
    free (data);
    printf ("[test_HashSet_Remove] OK\n");

}

void test_HashSet_Len ()
{
    HashSet *hashset = HashSet_Create (16);
    TypedVal *tv = Create_typed_value ();
    void* data = calloc (100, 8);

    size_t iters = 10;
    for (size_t i = 0; i < iters; i++) {
        *(int*) data = (int) i * 2;
        Set_typed_value (data, TYPE_INT, tv);
        HashSet_Add (hashset, tv);
        assert (HashSet_Len (hashset) == i+1);
    }
    assert (hashset->count == iters);
    assert (HashSet_Len (hashset) == iters);
    
    HashSet_Destroy (&hashset);
    Destroy_typed_value (&tv);
    free (data);
    printf ("[test_HashSet_Len] OK\n");
}



int main()
{
    test_HashSet_Create ();
    test__Resize ();
    test_HashSet_Destroy ();
    test_HashSet_Add_and_Contains ();
    test_HashSet_Remove ();
    test_HashSet_Len ();

    return 0;
}