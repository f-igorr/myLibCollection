#ifndef _MY_LIST_H_
#define _MY_LIST_H_


/* DLL это двусвязный список (Double Linked List) */

typedef struct DLL DLL;
typedef struct lNode lNode;


DLL* DLL_Create ();
void DLL_Clear (DLL* list);
void DLL_Destroy (DLL **list);
void DLL_Insert_Head (DLL *list, void *pdata);
void DLL_Insert_Tail (DLL *list, void *pdata);
bool DLL_Remove_Head (DLL* list, void** data);
bool DLL_Remove_Tail (DLL* list, void** data);
lNode* DLL_forward (lNode* node);
lNode* DLL_backward(lNode *node);
size_t DLL_Size (DLL*list);
void* DLL_Search_indx (DLL* list, long int indx);
void DLL_Print (DLL* list);


#endif