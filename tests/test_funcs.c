#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "funcs.h"


/* TEST functions from FUNCS.c */



static void test_djb2_hash_str ()
{
    char s1 [] = "abmavmi=q3954it0q-5jgm";
    char *s2 = malloc ((strlen (s1) + 1) * sizeof (char));
    char *s3 = malloc ((strlen (s1) + 1) * sizeof (char));
    
    strcpy (s2, s1); // s2 == s1
    strcpy (s3, s1);
    s3[0] = s3[1]; // s3 != s1

    size_t res1 =  djb2_hash_str (s1);
    size_t res2 =  djb2_hash_str (s2);
    size_t res3 =  djb2_hash_str (s3);
    
    printf ("[TEST 1] reults: 1: %zu, 2: %zu, 3: %zu\n", res1, res2, res3);

    assert (res1 == res2);
    assert (res3 != res1);

    free (s2);
    free (s3);

    printf ("[test_djb2_hash_str] OK\n");
}


/* test my_strdup() */
static void test_my_strdup ()
{
    char s1 [] = "qwerty";
    char *s2 = my_strdup (s1);

    printf ("[TEST 2] OK: s1 = %s, s2 = %s\n", s1, s2);

    assert (strcmp (s1, s2) == 0); // строки должеы быть равны
    assert (s1 != s2); // но должны лежать в разных участках памяти

    free (s2);
    printf ("[test_my_strdup] OK\n");
}



int main ()
{
    test_djb2_hash_str ();
    test_my_strdup ();

    return 0;
}