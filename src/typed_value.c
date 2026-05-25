#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


#include "typed_value.h"
#include "funcs.h"



typedef struct TypedVal{
    Type      type;
    union {
        int    i_val;
        float  f_val;
        double d_val;
        char*  s_val;
    } data;
} TypedVal;



/* очищение структуры TypedVal. контроль памяти
    принимает: указатель. НЕ ДОЛЖЕН быть указателем на строку-литерал
    если ранее хранился указ на строку. то освобождаем
    обнуляем все поля.
*/
void _Clean_typed_value (TypedVal *val)
{
    assert (val);
    if (val->type == TYPE_STRING && val->data.s_val)
        free (val->data.s_val);
    memset(val, 0, sizeof(TypedVal)); // обнуляем полностью, type -> TYPE_NONE, s_val -> NULL
    assert (val->data.s_val == NULL);
}


/* создание структуры
    возвращает указатель на новую структуру
*/
TypedVal* Create_typed_value (void)
{
    TypedVal* val = (TypedVal*) calloc (1, sizeof (TypedVal));
    assert (val);
    return val;
}


/*  уничтожение структуры
    перед уничтожением - очищение (возможно хранили указ на строку)
*/
void Destroy_typed_value (TypedVal **val)
{
    assert (val);
    assert (*val);
    _Clean_typed_value (*val);
    free (*val);
    *val = NULL;
}


/* сравнение двух структур TypedVal (по типу и значению)
    принимает указатели
    возвращает: true если [тип определен и типы равны и значения равны], иначе false
*/
bool Equal_typed_value (const TypedVal *v1, const TypedVal *v2)
{
    assert (v1 && v2);
    if (v1->type != v2->type) 
        return false;
    switch (v1->type)
    {
        case TYPE_NONE  : break; // нельзя сравнивать данные этого типа
        case TYPE_INT   : return v1->data.i_val == v2->data.i_val;
        case TYPE_FLOAT : return v1->data.f_val == v2->data.f_val;
        case TYPE_DOUBLE: return v1->data.d_val == v2->data.d_val;
        case TYPE_STRING: return strcmp (v1->data.s_val, v2->data.s_val) == 0;
    }
    return false;
}



size_t Hash_typed_value(const TypedVal *v)
{
    assert(v);

    size_t hash = 5381;
    hash = hash * 33u ^ (size_t)v->type;

    switch (v->type)
    {
        case TYPE_NONE:  assert(0);  break;
        case TYPE_INT : hash = hash * 33u ^ (size_t)v->data.i_val; break;
        case TYPE_FLOAT: {
            uint32_t bits;
            memcpy(&bits, &v->data.f_val, sizeof(bits));
            hash = hash * 33u ^ bits;
            break;
        }

        case TYPE_DOUBLE: {
            uint64_t bits;
            memcpy(&bits, &v->data.d_val, sizeof(bits));
            hash = hash * 33u ^ bits;
            break;
        }

        case TYPE_STRING: {
            const unsigned char *str = (const unsigned char *)v->data.s_val;
            unsigned char c;
            while ((c = *str++))
                hash = ((hash << 5) + hash) + c;
            break;
        }
    }
    return hash;
}


/* присвоение типа и значения структуре
    принимает: указ на данные (src), тип этих данных, указ на структуру TypedVal
    если принимает TYPE_NONE, то допускается data == NULL, структура просто зануляется
    если TYPE_STRING то глубокое копирование
    */
void Set_typed_value (const void* data, const Type type, TypedVal *tval)
{
    assert (tval);
    _Clean_typed_value (tval);
    if (type == TYPE_NONE) 
        return;
    assert (data);
    tval->type = type;
    switch (type) {
        case TYPE_NONE  : break;
        case TYPE_INT   : tval->data.i_val = *(int*)    data; break;
        case TYPE_FLOAT : tval->data.f_val = *(float*)  data; break;
        case TYPE_DOUBLE: tval->data.d_val = *(double*) data; break;
        case TYPE_STRING: tval->data.s_val = my_strdup ((const char*) data); break;
    }
}


/* копирует значение TypedVal  */
void Get_typed_value (void *data, TypedVal *tval)
{
    assert (tval);
    switch (tval->type) {
        case TYPE_NONE  : data = NULL; break;
        case TYPE_INT   : *(int*)    data = tval->data.i_val; break;
        case TYPE_FLOAT : *(float*)  data = tval->data.f_val; break;
        case TYPE_DOUBLE: *(double*) data = tval->data.d_val; break;
        case TYPE_STRING: strcpy (data, tval->data.s_val); break;
    }
}


/* копирование структуры. с предварительной очисткой
    принимает: указатели
    если TYPE_STRING то глубокое копирование
    */
void Copy_typed_value (TypedVal* dst, const TypedVal *src)
{
    assert (dst && src);
    _Clean_typed_value (dst);
    if (src->type == TYPE_NONE) 
        return;
    dst->type = src->type;
    switch (src->type) {
        case TYPE_NONE  : break;
        case TYPE_INT   : dst->data.i_val = src->data.i_val; break;
        case TYPE_FLOAT : dst->data.f_val = src->data.f_val; break;
        case TYPE_DOUBLE: dst->data.d_val = src->data.d_val; break;
        case TYPE_STRING: dst->data.s_val = my_strdup (src->data.s_val); break;
    }
}


/* печать структуры: тип, значение
    принимает указатель
*/
void typed_value_print (const TypedVal *val)
{
    assert (val);
    printf ("typed_val: type = ");
    switch (val->type)
    {
        case TYPE_NONE  : printf ("TYPE_NONE  , data = NONE\n"); break;
        case TYPE_INT   : printf ("TYPE_INT   , data = %d\n" , val->data.i_val); break;
        case TYPE_FLOAT : printf ("TYPE_FLOAT , data = %f\n" , val->data.f_val); break;
        case TYPE_DOUBLE: printf ("TYPE_DOUBLE, data = %lf\n", val->data.d_val); break;
        case TYPE_STRING: printf ("TYPE_STRING, data = %s\n" , val->data.s_val); break;
    }
}