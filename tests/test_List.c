#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "myList.h"


/* структура показана только для тестов */
//==================================
typedef struct lNode {
    void *data;
    struct lNode *prev;
    struct lNode *next;
} lNode;


typedef struct DLL {
    lNode *head;
    lNode *tail;
    size_t count; 
} DLL;
//==================================



void test_DLL_Create ()
{
    DLL *list = DLL_Create ();
    assert (list != NULL);
    assert (list->head == NULL);
    assert (list->tail == NULL);
    assert (list->count == 0);
    DLL_Destroy (&list);

    printf ("[test_DLL_Create] OK\n");
}


void test_DLL_Clear ()
{
    DLL *list = DLL_Create ();
    assert (list != NULL);

    int n = 2;
    int *data = malloc (n * sizeof(int));
    for (int i = 0; i < n; i++) {
        data [i] = i + 1;
        DLL_Insert_Head (list, (void*) (data + i));
        DLL_Insert_Tail (list, (void*) (data + i));
    }
    DLL_Clear (list);
    assert (list != NULL);
    assert (list->head == NULL);
    assert (list->tail == NULL);
    assert (list->count == 0);

    DLL_Destroy (&list);
    free (data);
    printf ("[test_DLL_Clear] OK\n");
}


void test_DLL_Destroy ()
{
    DLL *list = DLL_Create ();
    assert (list != NULL);

    size_t n = 10;
    int *data = malloc (n * sizeof(int));
    for (size_t i = 0; i < n; i++) {
        data [i] = i + 1;
        DLL_Insert_Head (list, (void*) (data + i));
    }
    assert (list->count == n);
    DLL_Destroy (&list);
    assert (list == NULL);
    for (size_t i = 0; i < n; i++) {
        assert (data [i] == (int) i + 1);
    }
    free (data);
    printf ("[test_DLL_Destroy] OK\n");
}


void test_DLL_Insert_Head ()
{
    DLL *list = DLL_Create ();
    assert (list != NULL);

    size_t n = 10;
    int *data = malloc (n * sizeof(int));
    for (size_t i = 0; i < n; i++) {
        data [i] = i + 1;
        DLL_Insert_Head (list, (void*) (data + i));
        assert (list->count == 1 + i);
        assert (list->head->prev == NULL);
        assert (list->head->data == data + i );
    }
    assert (list->count == n);
    DLL_Destroy (&list);
    free (data);
    printf ("[test_DLL_Insert_Head] OK\n");
}


void test_DLL_Insert_Tail ()
{
    DLL *list = DLL_Create ();
    assert (list != NULL);

    size_t n = 10;
    int *data = malloc (n * sizeof(int));
    for (size_t i = 0; i < n; i++) {
        data [i] = i + 1;
        DLL_Insert_Tail (list, (void*) (data + i));
        assert (list->count == 1 + i);
        assert (list->tail->next == NULL);
        assert (list->tail->data == data + i );
    }
    assert (list->count == n);
    DLL_Destroy (&list);
    free (data);
    printf ("[test_DLL_Insert_Tail] OK\n");
}


void test_DLL_Remove_Head ()
{
    DLL *list = DLL_Create ();
    assert (list != NULL);

    size_t n = 3;
    int *data = malloc (n * sizeof(int));
    void **out = malloc (n * sizeof(void*));

    for (size_t i = 0; i < n; i++) {
        data [i] = i + 1;
        DLL_Insert_Tail (list, (void*) (data + i));
    }
    for (size_t i = 0; i < n; i++) {
        DLL_Remove_Head (list, out+i);
        assert (list->count == n - 1 - i);
        if (list->count > 0)
            assert (list->head->prev == NULL);
        else
            assert (list->head == NULL);
        assert (*(out + i) == (data + i));
    }
    assert (list->count == 0);
    DLL_Destroy (&list);
    free (data);
    free (out);
    printf ("[test_DLL_Remove_Head] OK\n");
}


void test_DLL_Remove_Tail ()
{
    DLL *list = DLL_Create ();
    assert (list != NULL);

    size_t n = 3;
    int *data = malloc (n * sizeof(int));
    void **out = malloc (n * sizeof(void*));

    for (size_t i = 0; i < n; i++) {
        data [i] = i + 1;
        DLL_Insert_Tail (list, (void*) (data + i));
    }
    for (size_t i = 0; i < n; i++) {
        DLL_Remove_Tail (list, out + i);
        assert (list->count == n - 1 - i);
        if (list->count > 0)
            assert (list->tail->next == NULL);
        else
            assert (list->tail == NULL);
        assert (*(out + i) == (data + n - 1 - i));
    }
    assert (list->count == 0);
    DLL_Destroy (&list);
    free (data);
    free (out);
    printf ("[test_DLL_Remove_Tail] OK\n");
}


void test_DLL_forward_backward ()
{
    DLL *list = DLL_Create ();
    assert (list != NULL);

    size_t n = 10;
    int *data = malloc (n * sizeof(int));
    for (size_t i = 0; i < n; i++) {
        data [i] = i + 1;
        DLL_Insert_Head (list, (void*) (data + i));
    }
    lNode* node = list->head;
    void* pd = data;
    for ( ;node; ) {
        assert (node->data = pd++);
        node = DLL_forward (node);
    }
    node = list->tail;
    for ( ;node; ) {
        assert (node->data = pd--);
        node = DLL_backward (node);
    }
    DLL_Destroy (&list);
    free (data);
    printf ("[test_DLL_forward_backward] OK\n");
}


void test_DLL_Size ()
{
    DLL *list = DLL_Create ();
    assert (list != NULL);

    size_t size = DLL_Size (list);
    assert (size == 0);

    size_t n = 10;
    int *data = malloc (n * sizeof(int));
    for (size_t i = 0; i < n; i++) {
        data [i] = i + 1;
        DLL_Insert_Head (list, (void*) (data + i));
    }
    size = DLL_Size (list);
    assert (size == n);

    DLL_Destroy (&list);
    free (data);
    printf ("[test_DLL_size] OK\n");
}


void test_DLL_Search_indx ()
{
    DLL *list = DLL_Create ();
    assert (list != NULL);

    size_t n = 10;
    int *data = malloc (n * sizeof(int));
    void **out = malloc (n * sizeof(void*));

    for (size_t i = 0; i < n; i++) {
        data [i] = i + 1;
        DLL_Insert_Tail (list, (void*) (data + i));
    }
    for (size_t i = 0; i < n; i++) {
        out[i] = DLL_Search_indx (list, i);
        assert (*((int*)out[i]) == data [i]);
    }
    DLL_Destroy (&list);
    free (data);
    free (out);
    printf ("[test_DLL_Search_indx] OK\n");
}


int main ()
{
    test_DLL_Create ();
    test_DLL_Insert_Head ();
    test_DLL_Insert_Tail ();
    test_DLL_Remove_Head ();
    test_DLL_Remove_Tail ();
    test_DLL_forward_backward ();
    test_DLL_Size ();
    test_DLL_Search_indx ();
    test_DLL_Clear ();
    test_DLL_Destroy ();
    
    return 0;
}