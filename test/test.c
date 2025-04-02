#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include <assert.h>

#include <alloc/arena.h>
#include "csv/csv.h"


#define IRIS_FILE "assets/Iris.csv"


int main(void) {
    ArenaAlloc alloc = arena_alloc(1024*30);

	FILE * f = fopen(IRIS_FILE, "r");
	assert(f != NULL);

    fseek(f, 0L, SEEK_END);
    size_t length = ftell(f);
    fseek(f, 0L, SEEK_SET);

    char * csv_string = new(ALLOC(&alloc), length + 1);
    fread(csv_string, sizeof(char), length, f);
	fclose(f);

    CSV iris_csv = csv_deserialize(csv_string, ',', ALLOC(&alloc));
    
    /*
    csv_show(&iris_csv, ',', stdout);
    */
    
    iterate(csv_to_iterator(&iris_csv), CSV_Record*, record, {
        if(iterator.index > csv_columns(&iris_csv)) {
            if((iterator.index % csv_columns(&iris_csv)) == 1) {
                printf("%.*s\n", (int) record->length, record->c_str);
            }
        }
    });

    finalize(ALLOC(&alloc));
    printf("Program exit..\n");
    return EXIT_SUCCESS;
}


