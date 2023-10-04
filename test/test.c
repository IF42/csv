#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include <throw.h>

#include "../src/csv.h"


#define IRIS_FILE "Iris.csv"


int
main(void)
{
	FILE * f = fopen(IRIS_FILE, "r");

	if(f == NULL)
		throw("Can't open file '%s' for readig.\n", IRIS_FILE);

	CSV_Reader * reader = csv_reader_new(f, ',');
	
	if(reader == NULL)
		throw("CSV_Reader initialization error.\n");

	do
	{
		CSV_Cell * cell;

		while((cell = csv_reader_cell(reader)) != NULL)
			printf("%.*s ", cell->length, cell->value);

		printf("\n");
	}
	while(csv_reader_row(reader) == true);

	fclose(f);

    printf("Program exit..\n");
    return EXIT_SUCCESS;
}


