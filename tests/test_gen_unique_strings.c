#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <limits.h>

#include "gen_unique_strings.h"




int main ()
{
    int count [] = {1296, 1297, 0, 100, LIMIT_COUNT_STR, LIMIT_COUNT_STR + 1, 100              };
    int len   [] = {2   , 2   , 2 , 0 , LIMIT_LEN_STR  , 2                  , LIMIT_LEN_STR + 1};

    int correct_status [] = {FGUS_SUCCESS, 
                            FGUS_LIMIT_COMBINATIONS, 
                            FGUS_INVALID_ARG, 
                            FGUS_INVALID_ARG,
                            FGUS_SUCCESS,
                            FGUS_LIMIT_COUNT_STR,
                            FGUS_LIMIT_LEN_STR
                        };

    const int n = sizeof (count) / sizeof (int);
    int max_count = 0, max_len = 0;
    for (int i = 0; i < n; i++) {
        max_count = count[i] > max_count ? count[i] : max_count;
        max_len = len[i] > max_len ? len[i] : max_len;
    }

    //printf ("max_count = %d, max_len = %d\n", max_count, max_len);

    char ** res = calloc ( max_count, sizeof (char*));
    assert (res != NULL);
    for (int i =0 ; i < max_count; i++) {
        res[i] = calloc (max_len + 1, sizeof (char)); // !!!   len + 1 !!!!
        assert (res[i]);
    }
    
    printf ("memory alloc OK\n");

    int status;
    int TEST_RESULT = 0;
    for (int i = 0; i < n; i++) {
        //for (int k = 0; k < count[i]; k++) {
        //    res [k][len[n]] = 0;
        //}
        status = gen_unique_strings (res, count[i], len[i]);
        printf ("test with (count = %u, len = %u ): ", count[i], len[i]);
        if (status != correct_status [i])
            printf ("[ERROR]: status (%d) != correct_status[%d]. FAIL\n", status,i);
        else {
            printf ("OK\n");
            TEST_RESULT++;
        }
    }

    printf ("PASSED %d FROM %d\n", TEST_RESULT, n);

    //
    for (int i = 0; i < max_count; i++) {
        free (res[i]);
    }
    free (res);

    return 0;
}
