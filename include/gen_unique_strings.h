#ifndef _GEN_UNIQUE_STR_H_
#define _GEN_UNIQUE_STR_H_


typedef enum {
    FGUS_SUCCESS = 0,
    FGUS_INVALID_ARG,
    FGUS_LIMIT_COUNT_STR,
    FGUS_LIMIT_LEN_STR,
    FGUS_LIMIT_COMBINATIONS
} FGUS_STATUS;


#define LIMIT_COUNT_STR 1000000
#define LIMIT_LEN_STR 100


//char** gen_unique_strings (const int count_s, const int len_s);
FGUS_STATUS gen_unique_strings (char ** arr_str, const int count_s, const int len_s);


#endif