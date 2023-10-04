#include "csv.h"

#include <string.h>
#include <stdlib.h>


struct CSV_Reader
{
	CSV_Cell cell;

	char separator;

    size_t index;
    size_t length;

    char array[];
};


CSV_Reader *
csv_reader_string_new(
	char * string
	, char separator)
{
	size_t memsize = strlen(string);

	CSV_Reader * self = malloc(sizeof(CSV_Reader) + memsize);

	if(self != NULL)
	{
		*self = (CSV_Reader)
		{
			.length      = memsize
			, .separator = separator
			, .index     = 0
		};

		memcpy(self->array, string, memsize);
	}

    return self;
}


CSV_Reader *
csv_reader_FILE_new(
	FILE * f
	, char separator)
{
    fseek(f, 0L, SEEK_END);
    size_t memsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    CSV_Reader * self = malloc(sizeof(CSV_Reader) + (memsize + 1));

	if(self != NULL)
	{
		*self = (CSV_Reader)
		{
			.length      = memsize
			, .separator = separator
			, .index     = 0
		};

		fread(self->array, memsize, sizeof(char), f);
	}
	
	return self;
}


bool
csv_reader_row(CSV_Reader * self)
{
	if(self->array[self->index] == '\n')
	{
		self->index++;
		return true;
	}

	return false;
}


CSV_Cell * 
csv_reader_cell(CSV_Reader * self)
{
	while(self->array[self->index] != '\0' 
		&& self->array[self->index] != '\n')
	{
		if(self->array[self->index] == self->separator)
		{
			self->index++;
			continue;
		}
		else
		{
			self->cell.length = self->index;
			self->cell.value  = &self->array[self->index];

			while(self->array[self->index] != self->separator 
                    && self->array[self->index] != '\n' 
					&& self->array[self->index] != '\0')
			{
				self->index++;
			}

			self->cell.length = self->index - self->cell.length;

			return &self->cell;
		}
	}

	return NULL;
}



void
csv_reader_delete(CSV_Reader * self)
{
	if(self != NULL)
		free(self);
}



