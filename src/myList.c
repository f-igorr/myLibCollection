#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "myList.h"


/* Данная реализация хранит в нодах только ссылки на внешние данные!!! */


/* ====  TODO =======
    можно либо сделать отдельную реализацию листа для работы не с ссылками на данные, 
    а с копиями данных (полное влядение данными),
    либо сделать лист универсальным, с внутренним параметром (кот устанавливается 
    при каждом создании листа), определяющим метод работы с данными.
*/



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


/* create new EMPTY list
    return pointer to new EMPTY list
*/
DLL* DLL_Create ()
{
    DLL* new_list = (DLL*) calloc (1, sizeof (DLL));
    assert (new_list != NULL);
    // calloc устанавливает все поля в 0. доп инициализация не нужна
    return new_list;
}


void DLL_Clear (DLL* list)
{
    assert (list != NULL);

    // список пуст, ничего не делаем
    if (list->count == 0) 
        return;

    // иначе список не пустой
    while (DLL_Remove_Head(list, NULL)) {}
    assert (list->count == 0);
    assert (list != NULL);
    return;
}

/* Полное удаление листа с предварит удалением всех элементов */
void DLL_Destroy (DLL **pplist)
{
    assert (pplist != NULL );
    DLL *list = *pplist;
    assert (list != NULL);

    // если передали пустой список
    if (list->count == 0) {
        assert (list->head == NULL && list->tail == NULL);
        free (list);
        *pplist = NULL;
        return;
    }
    // иначе список не пустой 
    while (DLL_Remove_Head(list, NULL)) {}
    assert (list->count == 0);
    free (list);
    *pplist = NULL;
    return;
}


void DLL_Insert_Head (DLL *list, void *data)
{
    assert (list != NULL); // data может быть NULL 

    lNode *new_node = (lNode*) calloc (1, sizeof (lNode)); // set all to 0
    assert (new_node != NULL);
    new_node->data = data;

    if (list->count == 0) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        new_node->next = list->head;
        new_node->next->prev = new_node;
        list->head = new_node;
    }
    list->count++;
    return;
}


void DLL_Insert_Tail (DLL *list, void *data)
{
    assert (list != NULL); // data может быть NULL 

    lNode *new_node = (lNode*) calloc (1, sizeof (lNode)); // set all to 0
    assert (new_node != NULL);
    new_node->data = data;

    if (list->count == 0) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        new_node->prev = list->tail;
        new_node->prev->next = new_node;
        list->tail = new_node;
    }
    list->count++;
    return;
}


/* out может быть NULL (при Clean и Destory) */
bool DLL_Remove_Head (DLL* list, void** out)
{
    assert (list != NULL);

    // список пуст, ничего не делаем
    if (list->count == 0) {
        // *out
        return false;
    }
    // если список НЕ пустой
    lNode *curr_head = list->head;

    // если 1 элемент в списке, то head == tail
    if (list->count == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = list->head->next;
        list->head->prev = NULL;
    }
    if (out)
        *out = curr_head->data;
    free (curr_head);
    list->count--;

    if (list->count > 0)
        assert (list->count  > 0 && list->head != NULL && list->tail != NULL);
    if (list->count == 0)
        assert (list->count == 0 && list->head == NULL && list->tail == NULL);

    return true;
}


/* out может быть NULL (при Clean и Destory) */
bool DLL_Remove_Tail (DLL* list, void** out)
{
    assert (list != NULL);

    // список пуст, ничего не делаем
    if (list->count == 0) 
        return false;

    // если список НЕ пустой
    lNode *curr_tail = list->tail;

    // если 1 элемент в списке, то head == tail
    if (list->count == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    if (out)
        *out = curr_tail->data;
    free (curr_tail);
    list->count--;

    //assert (list->count  > 0 && list->head != NULL && list->tail != NULL);
    //assert (list->count == 0 && list->head == NULL && list->tail == NULL);

    if (list->count > 0)
        assert (list->count  > 0 && list->head != NULL && list->tail != NULL);
    if (list->count == 0)
        assert (list->count == 0 && list->head == NULL && list->tail == NULL);

    return true;
}


lNode* DLL_forward(lNode *node) 
{
    return node->next;
}


lNode* DLL_backward(lNode *node) 
{
    return node->prev;
}


/* поиск элемента по индексу и возврат указателя на данные этого элемента
    indx может быть < 0, тогда отсчет с конца
*/
void* DLL_Search_indx (DLL* list, long int indx)
{
    if (indx == 0)
        return list->head->data;

    lNode *curr = NULL; 
    if (indx > 0) {
        assert ((size_t) indx < list->count);
        curr = list->head;
        for (; indx > 0; indx--)
            curr = DLL_forward(curr);
    }
    if (indx < 0) {
        assert ( (size_t) (-indx) <= list->count);
        curr = list->tail;
        for (; indx < 0; indx++)
            curr = DLL_backward(curr);
    }
    return curr->data;
}


size_t DLL_Size (DLL* list)
{
    return list->count;
}


void DLL_Print (DLL* list)
{
    printf ("\nList:\n");
    printf ("\tcount = %ld\n", list->count);
    printf ("\thead = %p\n", list->head);
    printf ("\ttail = %p\n", list->tail);
}