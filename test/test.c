#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include <assert.h>

#include <alloc/arena.h>
#include "csv/csv.h"


#define IRIS_FILE "assets/ats.csv"


int main(void) {
    ArenaAlloc alloc = arena_alloc(1024*50);

	FILE * f = fopen(IRIS_FILE, "r");
	assert(f != NULL);

    fseek(f, 0L, SEEK_END);
    size_t length = ftell(f);
    fseek(f, 0L, SEEK_SET);

    char * csv_string = new(ALLOC(&alloc), length + 1);
    fread(csv_string, sizeof(char), length, f);
	fclose(f);

    CSV batch_csv = csv_deserialize(csv_string, ',', ALLOC(&alloc));
    
    
    //csv_show(&iris_csv, ',', stdout);
    size_t columns = csv_columns(&batch_csv);

    iterate(csv_to_iterator(&batch_csv), CSV_Record*, record, {
        if(iterator.index > 800 && (iterator.index % columns) == 1) {
            printf("%s %.*s %f\n", CSV_TYPE(record->type), (int) record->length, record->c_str, strtod(record->c_str, NULL));
        }
    });
   
    finalize(ALLOC(&alloc));
    printf("Program exit..\n");
    return EXIT_SUCCESS;
}


