#ifndef _CSV_H_
#define _CSV_H_


#include <stdio.h>
#include <stdbool.h>


typedef struct CSV_Reader CSV_Reader;


typedef struct
{
    int length;
    char * value;
}CSV_Cell;


CSV_Reader *
csv_reader_string_new(
	char * string
	, char separator);


CSV_Reader *
csv_reader_FILE_new(
	FILE * f
	, char separator);


#define csv_reader_new(T, c)				\
	_Generic(								\
		(T)									\
		, char *: csv_reader_string_new		\
		, FILE *: csv_reader_FILE_new)		\
			((T), (c))

bool
csv_reader_row(CSV_Reader * self);


CSV_Cell *
csv_reader_cell(CSV_Reader * self);


void
csv_reader_delete(CSV_Reader * self);


#endif

