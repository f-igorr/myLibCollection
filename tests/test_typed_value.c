#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "typed_value.h"


// внутрянка раскрыта только для тестов
typedef struct TypedVal{
    Type      type;
    union {
        int    i_val;
        float  f_val;
        double d_val;
        char*  s_val;
    } data;
} TypedVal;



int test_Create_typed_value ()
{
    TypedVal *tval = Create_typed_value ();
    if (tval) {
        Destroy_typed_value (&tval);
        printf ("[test_Create_typed_value] OK\n");
        return 0; // OK
    }
    else return 1; // FAIL
}

int test_Set_typed_value ()
{
    printf ("[test_Set_typed_value] ");

    TypedVal *tval = Create_typed_value ();
    assert (tval);
    void *p = NULL;
    int type;

    // TYPE_NONE
    type = TYPE_NONE;
    int x = 10;
    p = (void*) &x;
    Set_typed_value (p, type, tval);
    if (tval->type != (Type) type || tval->data.i_val != 0 || tval->data.f_val != 0 || tval->data.d_val != 0 || tval->data.s_val != 0) {
        Destroy_typed_value (&tval);
        printf ("FAIL\n");
        return 1; // FAIL
    }
    
    // TYPE_INT
    type = TYPE_INT;
    int ix = 10;
    p = (void*) &ix;
    Set_typed_value (p, type, tval);
    if (tval->type != (Type) type || tval->data.i_val != ix) {
        Destroy_typed_value (&tval);
        printf ("FAIL\n");
        return 1; // FAIL
    }

    // TYPE_FLOAT
    type = TYPE_FLOAT;
    float fx = -100.0f;
    p = (void*) &fx;
    Set_typed_value (p, type, tval);
    if (tval->type != (Type) type || tval->data.f_val != fx) {
        Destroy_typed_value (&tval);
        printf ("FAIL\n");
        return 1; // FAIL
    }

    // TYPE_DOUBLE
    type = TYPE_DOUBLE;
    double dx = 0.999;
    p = (void*) &dx;
    Set_typed_value (p, type, tval);
    if (tval->type != (Type) type || tval->data.d_val != dx) {
        Destroy_typed_value (&tval);
        printf ("FAIL\n");
        return 1; // FAIL
    }

    // TYPE_STRING
    type = TYPE_STRING;
    char * s = "helloo";
    p = (void*) s;
    Set_typed_value (p, type, tval);
    if (tval->type != (Type) type || strcmp (tval->data.s_val, s) != 0) {
        Destroy_typed_value (&tval);
        printf ("FAIL\n");
        return 1; // FAIL
    }
    Destroy_typed_value (&tval);
    printf ("OK\n");
    return 0;
}

int test_Destroy_typed_value()
{
    printf ("[test_Destroy_typed_value] ");

    TypedVal *tval = Create_typed_value ();
    assert (tval);
    int x = 10;
    char *s = "helloo";
    Set_typed_value ((void*)&x, TYPE_INT, tval);
    Set_typed_value ((void*)s, TYPE_STRING, tval);
    
    //int *pi  = &(tval->data.i_val);
    //char *ps = tval->data.s_val;
    
    Destroy_typed_value (&tval);
    //Destroy_typed_value (&tval);

    if (tval) { // || *pi == x || strcmp (ps, s) == 0) {
        printf ("FAIL\n");
        return 1;
    }
    printf ("OK\n");

    return 0;
}


int test__Clean_typed_value ()
{
    printf ("[test__Clean_typed_value] ");

    TypedVal *tval = Create_typed_value ();
    void *p;

    int x = 10;    p = (void*) &x;
    Set_typed_value (p, TYPE_INT, tval);
    _Clean_typed_value (tval);
    if (tval->type || tval->data.i_val || tval->data.f_val || tval->data.d_val || tval->data.s_val) {
        printf ("FAIL\n");
        return 1; // FAIL
    }

    float y = -10.0f;    p = (void*) &y;
    Set_typed_value (p, TYPE_FLOAT, tval);
    _Clean_typed_value (tval);
    if (tval->type || tval->data.i_val || tval->data.f_val || tval->data.d_val || tval->data.s_val) {
        printf ("FAIL\n");
        return 1; // FAIL
    }

    double z = 10.0;    p = (void*) &z;
    Set_typed_value (p, TYPE_DOUBLE, tval);
    _Clean_typed_value (tval);
    if (tval->type || tval->data.i_val || tval->data.f_val || tval->data.d_val || tval->data.s_val) {
        printf ("FAIL\n");
        return 1; // FAIL
    }

    Set_typed_value ((void*)"helllooo", TYPE_STRING, tval);
    _Clean_typed_value (tval);
    if (tval->type || tval->data.i_val || tval->data.f_val || tval->data.d_val || tval->data.s_val) {
        printf ("FAIL\n");
        return 1; // FAIL
    }

    Destroy_typed_value (&tval);
    printf ("OK\n");

    return 0;
}

int test_Equal_typed_value ()
{
    printf ("[test_Equal_typed_value] ");

    TypedVal *tval1 = Create_typed_value ();
    TypedVal *tval2 = Create_typed_value ();

    void *p;

    int x = 10;    p = (void*) &x;
    Set_typed_value (p, TYPE_INT, tval1);
    Set_typed_value (p, TYPE_INT, tval2);
    if (!Equal_typed_value (tval1, tval2)) {printf ("FAIL\n");return 1;}
    x++;
    Set_typed_value (p, TYPE_INT, tval2);
    if (Equal_typed_value (tval1, tval2))  {printf ("FAIL\n");return 1;}

    float y = -10.0f;    p = (void*) &y;
    Set_typed_value (p, TYPE_FLOAT, tval1);
    Set_typed_value (p, TYPE_FLOAT, tval2);
    if (!Equal_typed_value (tval1, tval2))  {printf ("FAIL\n");return 1;}
    y += 1.0f;
    Set_typed_value (p, TYPE_FLOAT, tval2);
    if (Equal_typed_value (tval1, tval2))  {printf ("FAIL\n");return 1;}

    double z = 10.0;    p = (void*) &z;
    Set_typed_value (p, TYPE_DOUBLE, tval1);
    Set_typed_value (p, TYPE_DOUBLE, tval2);
    if (!Equal_typed_value (tval1, tval2))  {printf ("FAIL\n");return 1;}
    z += 5.0;
    Set_typed_value (p, TYPE_DOUBLE, tval2);
    if (Equal_typed_value (tval1, tval2))  {printf ("FAIL\n");return 1;}

    Set_typed_value ((void*)"helllooo", TYPE_STRING, tval1);
    Set_typed_value ((void*)"helllooo", TYPE_STRING, tval2);
    if (!Equal_typed_value (tval1, tval2))  {printf ("FAIL\n");return 1;}
    Set_typed_value ((void*)"hell", TYPE_STRING, tval2);
    if (Equal_typed_value (tval1, tval2))  {printf ("FAIL\n");return 1;}

    Destroy_typed_value (&tval1);
    Destroy_typed_value (&tval2);
    printf ("OK\n");

    return 0;
}

int test_Hash_typed_value ()
{
    #include <time.h>

    printf ("[test_Hash_typed_value] ");

    srand ((unsigned) time(NULL));
    TypedVal *a = Create_typed_value ();
    TypedVal *b = Create_typed_value ();

    int ix, r;
    float fx;
    double dx;
    char s [100];
    for (int i = 0; i < 100000; i++) {
        r = rand() - RAND_MAX / 2; // -> [ -1/2 RAND_MAX ; +1/2 RAND_MAX ]
        switch (i % 4 + 1) {
            case TYPE_INT   : ix =         r; Set_typed_value ((void*)&ix, TYPE_INT      , a); break;
            case TYPE_FLOAT : fx = (float) r; Set_typed_value ((void*)&fx, TYPE_FLOAT    , a); break;
            case TYPE_DOUBLE: dx = (double)r; Set_typed_value ((void*)&dx, TYPE_DOUBLE   , a); break;
            case TYPE_STRING: sprintf (s,"%d", r); Set_typed_value ((void*)s, TYPE_STRING, a); break;
        }
        Copy_typed_value (b, a);

        assert(Equal_typed_value(a, b));
        if (Hash_typed_value(a) != Hash_typed_value(b)) {printf ("FAIL\n");return 1;}
    }

    Set_typed_value ((void*)&((int){123}), TYPE_INT, a);
    Set_typed_value ((void*)&((int){123}), TYPE_INT, b);
    if (Hash_typed_value(a) != Hash_typed_value(b)) {printf ("FAIL\n");return 1;}

    Set_typed_value ((void*)&((int){123}), TYPE_INT, a);
    Set_typed_value ((void*)&((int){124}), TYPE_INT, b);
    if (Hash_typed_value(a) == Hash_typed_value(b)) {printf ("FAIL\n");return 1;}

    Set_typed_value ((void*)"abc", TYPE_STRING, a);
    Set_typed_value ((void*)"abc", TYPE_STRING, b);
    if (Hash_typed_value(a) != Hash_typed_value(b)) {printf ("FAIL\n");return 1;}

    Set_typed_value ((void*)"abc", TYPE_STRING, a);
    Set_typed_value ((void*)"abb", TYPE_STRING, b);
    if (Hash_typed_value(a) == Hash_typed_value(b)) {printf ("FAIL\n");return 1;}

    Set_typed_value ((void*)&((int){42}), TYPE_INT, a);
    Set_typed_value ((void*)"42", TYPE_STRING, b);
    if (Hash_typed_value(a) == Hash_typed_value(b)) {printf ("FAIL\n");return 1;}

    Destroy_typed_value (&a);
    Destroy_typed_value (&b);
    printf ("OK\n");
    return 0;
}

int test_Get_typed_value ()
{
    printf ("[test_Get_typed_value] ");

    TypedVal *a = Create_typed_value ();

    int    ix = 10, iy;
    float  fx = -5.55f, fy;
    double dx = 0.999, dy;
    char   sx [] = "qwerty";
    char   sy [20] = {0};

    Set_typed_value ((void*)&ix, TYPE_INT   , a);
    Get_typed_value ((void*)&iy, a);
    if (ix != iy) {printf ("FAIL\n");return 1;}

    Set_typed_value ((void*)&fx, TYPE_FLOAT , a);
    Get_typed_value ((void*)&fy, a);
    if (fx != fy) {printf ("FAIL\n");return 1;}

    Set_typed_value ((void*)&dx, TYPE_DOUBLE, a);
    Get_typed_value ((void*)&dy, a);
    if (dx != dy) {printf ("FAIL\n");return 1;}

    Set_typed_value ((void*)sx, TYPE_STRING, a);
    Get_typed_value ((void*)sy, a);
    if (strcmp (sx, sy) != 0) {printf ("FAIL\n");return 1;}

    Destroy_typed_value (&a);
    printf ("OK\n");
    return 0;
}


int main ()
{
    int result = 0;
    result += test_Create_typed_value ();
    result += test_Set_typed_value ();
    result += test__Clean_typed_value();
    result += test_Destroy_typed_value ();
    result += test_Equal_typed_value ();
    result += test_Hash_typed_value ();
    result += test_Get_typed_value ();

    printf ("result  = %d\n", result);

    return 0;
}