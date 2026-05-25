#ifndef _TYPED_VALUE_H_
#define _TYPED_VALUE_H_

#include <stddef.h>
#include <stdbool.h>


// structs
typedef enum {
    TYPE_NONE = 0,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_STRING
} Type;

typedef struct TypedVal TypedVal;


// functions
void _Clean_typed_value (TypedVal *val);
TypedVal* Create_typed_value (void);
void Destroy_typed_value (TypedVal **val);
bool Equal_typed_value (const TypedVal *v1, const TypedVal *v2);
size_t Hash_typed_value (const TypedVal *v);
void Set_typed_value (const void* data, const Type type, TypedVal *tval);
void Get_typed_value (void *data, TypedVal *tval);
void Copy_typed_value (TypedVal* dst, const TypedVal *src);
void typed_value_print (const TypedVal *val);


#endif