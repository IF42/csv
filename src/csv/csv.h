#ifndef _CSV_H_
#define _CSV_H_


#include <stdio.h>
#include <stdbool.h>
#include <alloc/alloc.h>
#include <iterator/iterator.h>


typedef enum {
    CSV_Type_String
    , CSV_Type_Number
    , CSV_Type_Bool
    , CSV_Type_Null
} CSV_Type;


#define CSV_TYPE(T) \
    (T) == CSV_Type_String ? "CSV_Type_String" : \
    (T) == CSV_Type_Number ? "CSV_Type_Number" : \
    (T) == (CSV_Type_Bool) ? "CSV_Type_Bool" : \
    (T) == (CSV_Type_Null) ? "CSV_Type_Null": \
                            "Undefined"


typedef struct {
    CSV_Type type;
    size_t length;
    char * c_str;
} CSV_Record;


typedef struct {
    Alloc * alloc;
    size_t row;
    size_t capacity;
    size_t size;
    CSV_Record * record;
} CSV;


CSV csv(Alloc * alloc);


CSV csv_deserialize(char * csv_string, char delimite,  Alloc * alloc);


size_t csv_rows(CSV * self);


size_t csv_columns(CSV * self);


Iterator csv_to_iterator(CSV * csv);


CSV_Record * csv_at(CSV * self, size_t row, size_t column);


void csv_show(CSV * self, char semicolon, FILE * stream);

void csv_finalize(CSV * self);

#endif

