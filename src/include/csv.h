#ifndef _CSV_H_
#define _CSV_H_


#include <numc/numc.h>

#include <stdbool.h>
#include <stdint.h>
#include <version.h>


#define __version_csv__ DEFINE_VERSION(0, 1, 0)


typedef struct
{
    char *** csv_grid;
    T_Shape shape_struct;
}T_CSV;


typedef union
{
    T_CSV  csv;
    Tensor tensor;
}CSV;


typedef struct
{
    bool is_value;
    CSV * value;
}O_CSV;


O_CSV 
csv_read(
    char * path
    , char separator);


O_CSV
csv_parse(
    char * csv_string
    , size_t length
    , char separator);





void
csv_delete(CSV * self);

#endif

