/**
** @file csv.c
*/ 

#include "include/csv.h"

#include <stdlib.h>

typedef struct
{
    char peek;
    char c;
    char separator;
    size_t index;
    size_t length;
    char * csv_string;
}CsvParser;


O_CSV 
csv_read(
    char * path
    , char separator)
{
    (void) path;
    (void) separator;

    return (O_CSV) {.is_value = false};
}


char *
parser_next(CsvParser * self)
{
    return NULL;
}


O_CSV
csv_parse(
    char * csv_string
    , size_t length
    , char separator)
{
    Tensor * csv_tensor = 
        tensor_new(1, (uint32_t[]){1}, sizeof(char*));

    CsvParser parser = 
        {
            .peek = csv_string[1]
            , .c = csv_string[0]
            , .index = 0
            , .length = length
            , .separator = separator
            , .csv_string = csv_string
        };

    char * next_ceel = NULL;

    while((next_ceel = parser_next(&parser)) != NULL)
    {

    }

    return (O_CSV) {.is_value = false};
}


void
csv_delete(CSV * self)
{
    for(size_t i = 0; i < tensor_size(&self->tensor); i++)
        free(self->tensor.vector);

    tensor_delete(&self->tensor);
}



