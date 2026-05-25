#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>


#include "gen_unique_strings.h"
#include "myHashSet.h"
//#include "typed_value.h"



/*  пересчет десятичного числа в base систему исчисления.
    num:  десятичное число которое надо разложить на разряды
    base: база исчисления (макс знач в разряде)
    len: требуемая длина массива результата (кол разрядов = длина строки)
    arr: массива результата
*/
static void _decompose (int num, const int base, const int len, int* arr)
{
    assert (arr != NULL);

    for (int i = 0; i < len; i++) {
        arr[i] = 0;
    }
    int i = len - 1;
    while (num) {
        arr[i] = num % base;
        num /= base;
        i--;
    }
}


/* полный перебор комбинаций символов
    принимает 
        arr_str: указ на массив строк
        count_s: кол-во строк
        len_s: длина строк
        SIMBOLS: указ-ль на массив используемых символов
        N_SIMBOLS */
static void _full_combo (char** arr_str, const int count_s, const int len_s, const char* SIMBOLS, const int N_SIMBOLS)
// count_s: кол-во строк 
// len_s  : длина строк (без '\0')
{
    int ind_simbol [len_s];
    int i;
    for (int n = 0; n < count_s; n++) {
        _decompose (n, N_SIMBOLS, len_s, ind_simbol);
        for (i = 0; i < len_s; i++) {
            arr_str [n][i] = SIMBOLS [ind_simbol[i]];
        }
    }
    return;
}


/* вычисляем макс длину строки для проверки на возможность полного перебора для данного N_SIMBOLS
    принимает кол-во используемых символов
    возвращает макс длину строки для которой возможен полный перебор комбинаций
*/
static int max_len_string (const int N_SIMBOLS)
{
    int ret = 0;
    int limit = INT_MAX;
    while (1) {
        limit /= N_SIMBOLS;
        if (limit == 0) break;  
        ret++;
    }
    return ret;
}


/* генерация  count_s УНИКАЛЬНЫХ строк len_s символов из заданного набора SIMBOLS
    !!!  ОГРАНИЧЕНИЕ КОЛ-ВА СТРОК = 1 000 000
    !!!  ОГРАНИЧЕНИЕ ДЛИНЫ СТРОК = 100
    принимает:
        arr_str: массив указателей на строки. память уже должна быть выделена, arr_str[len_s] == '\0'
        count_s: требуемое кол-во строк
        len_s: требуемая кол-во символов в строках
    заполняет массив УНИКАЛЬНЫМИ строками случ символов из набора символов 
    возвращает статус
    */
FGUS_STATUS gen_unique_strings (char **arr_str, const int count_s, const int len_s)
{
    //================================================================
    static const char SIMBOLS [] = 
    {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };
    static const int N_SIMBOLS = (int) (sizeof (SIMBOLS) / sizeof (char));
    
    //==============================================================


    // CHECK ARGs
    if (arr_str == NULL) {
        printf ("[FGUS_INVALID_ARG] arr_str == NULL\n");
        return FGUS_INVALID_ARG;
    }
    for (int i = 0; i < count_s; i++) {
        if (arr_str[i] == NULL) {
            printf ("[FGUS_INVALID_ARG] arr_str[%d] == NULL\n", i);
            return FGUS_INVALID_ARG;
        }
        arr_str[i][len_s] = '\0';
    }
    if ( count_s <= 0 || len_s <= 0) {
        printf ("[FGUS_INVALID_ARG] count_s or len_s <= 0\n");
        return FGUS_INVALID_ARG;
    }
    if (count_s > LIMIT_COUNT_STR) { // 1 000 000
        printf("[FGUS_LIMIT_COUNT_STR] Превышен лимит кол-ва строк (%d))\n", LIMIT_COUNT_STR);
        return FGUS_LIMIT_COUNT_STR;
    }
    if (len_s > LIMIT_LEN_STR) { // 100
        printf("[FGUS_LIMIT_LEN_STR] Превышен лимит длины строки (%d))\n", LIMIT_LEN_STR);
        return FGUS_LIMIT_LEN_STR;
    }
    int max_len = max_len_string (N_SIMBOLS); // 
    int teor = 0;
    if (len_s <= max_len) { // если истина то можем посчитать теор кол комбинаций. иначе возможно переполнение
        teor =  (int) pow ( (double) N_SIMBOLS, (double) len_s);
        if (count_s > teor) {
            printf("[FGUS_LIMIT_COMBINATIONS] Требуемое кол-во строк (%d) длиной (%d) превышает теор кол-во комбинаций (%d) из (%d) символов \n", count_s, len_s, teor, N_SIMBOLS);
            return FGUS_LIMIT_COMBINATIONS;
        }
    }
     
    if (count_s == teor)
        _full_combo (arr_str, count_s, len_s, SIMBOLS, N_SIMBOLS);
    else {
        srand( (unsigned int) time(NULL));
        HashSet *hashset  = HashSet_Create (count_s);
        char *s = calloc (len_s + 1, sizeof (char));
        assert (s);
        //s[len_s] = '\0';
        TypedVal *tval = Create_typed_value ();
        int n = 0;
        while ( (int) HashSet_Len(hashset) < count_s) {
            for (int i = 0; i < len_s; i++) {
                s[i] = SIMBOLS [rand() % N_SIMBOLS];
            }
            Set_typed_value ( (void*) s, TYPE_STRING, tval);
            if (HashSet_Add (hashset, tval)) {
                memcpy (arr_str[n], s, strlen(s)); // здесь memcpy быстрее чем strcpy
                n++;
            }
        }
        HashSet_Destroy (&hashset);
        Destroy_typed_value (&tval);
        free (s);
    }
    return FGUS_SUCCESS;
}