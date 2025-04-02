#include "csv.h"
#include <string.h>
#include <ctype.h>


typedef enum {
    CSV_TokenType_EOF
    , CSV_TokenType_EOL
    , CSV_TokenType_String
    , CSV_TokenType_Number
    , CSV_TokenType_Bool
    , CSV_TokenType_NULL
} CSV_TokenType;


typedef struct {
    CSV_TokenType type;
    size_t length;
    char * c_str;
} CSV_Token;


typedef struct {
    char delimiter;
    size_t index;
    char * c_str;
} CSV_Lexer;


static void skip_whitespace(CSV_Lexer * self) {
    while(self->c_str[self->index] != '\0' && isspace((int) self->c_str[self->index])) {
        self->index ++;
    }
}


static inline void skip_delimiter(CSV_Lexer * self) {
    if(self->c_str[self->index] == self->delimiter) {
        self->index++;
    }
}


static inline bool is_delimiter(CSV_Lexer * self) {
    if(self->c_str[self->index] == '\0' 
            || self->c_str[self->index] == self->delimiter 
            || self->c_str[self->index] == '\r' 
            || self->c_str[self->index] == '\n') {
        return true;
    } else {
        return false;
    }
}


CSV_Token csv_lexer_next(CSV_Lexer * self) {
    if(self->c_str[self->index] == '\0') {
        return (CSV_Token) {.type = CSV_TokenType_EOF};
    } else if(self->c_str[self->index] == '\r') {
        self->index ++;
    } 

    if(self->c_str[self->index] == '\n') {
        self->index++;
        return (CSV_Token) {.type = CSV_TokenType_EOL};
    } else {
        skip_whitespace(self);
    }

    if(self->c_str[self->index] == self->delimiter) {
        self->index ++;
        return (CSV_Token) {.type = CSV_TokenType_NULL};
    } else if(strncasecmp(&self->c_str[self->index], "true", 4) == 0) {
        CSV_Token token = {.type = CSV_TokenType_Bool, .length = 4, .c_str = &self->c_str[self->index]};
        self->index += 4;
        return token;
    } else if(strncasecmp(&self->c_str[self->index], "false", 5) == 0) {
        CSV_Token token = {.type = CSV_TokenType_Bool, .length = 5, .c_str = &self->c_str[self->index]};
        self->index += 5;
        return token;
    } else if(self->c_str[self->index] == '"') {
        self->index ++;
        CSV_Token token = {.type = CSV_TokenType_String, .length = self->index, .c_str = &self->c_str[self->index]};
        while(is_delimiter(self) == false && self->c_str[self->index] != '"') {
            self->index ++;
        }
        token.length = self->index - token.length;
        return token;
    } else if(isdigit((int) self->c_str[self->index]) 
            || ((self->c_str[self->index] == '+' 
                    || self->c_str[self->index] == '-' 
                    || self->c_str[self->index] == '.') 
                && isdigit((int) self->c_str[self->index + 1]))) {
        CSV_Token token = {.type = CSV_TokenType_Number, .length = self->index, .c_str = &self->c_str[self->index]};

        while(is_delimiter(self) == false) {
            if(isalpha((int) self->c_str[self->index])) {
                token.type = CSV_TokenType_String;
            } 
            self->index++;
        }

        token.length = self->index - token.length;
        return token;
    } else {
        CSV_Token token = {.type = CSV_TokenType_String, .length = self->index, .c_str = &self->c_str[self->index]};
        while(is_delimiter(self) == false) {
            self->index ++;
        }
        token.length = self->index - token.length;

        if(token.length == 0) {
            token.type = CSV_TokenType_NULL;
        }
        return token;
    }
}


CSV csv(Alloc * alloc) {
    return (CSV) {
        .alloc = alloc
    };
}


static void csv_append(CSV * self, CSV_Record record) {
    if(self->size >= self->capacity) {
        self->capacity = (self->capacity + 1) * 2;
        self->record = resize(self->alloc, self->record, self->capacity);
    }

    self->record[self->size++] = record;
}


CSV csv_deserialize(char * csv_string, char delimiter, Alloc * alloc) {
    CSV_Lexer lexer = {.delimiter = delimiter, .c_str = csv_string};
    CSV_Token token = {.type = CSV_TokenType_EOF};
    CSV csv_inst = csv(alloc);
    
    while((token = csv_lexer_next(&lexer)).type != CSV_TokenType_EOF) {
        switch(token.type) {
            case CSV_TokenType_EOL:
                csv_inst.row ++;
                continue;
            case CSV_TokenType_String:
                csv_append(&csv_inst, (CSV_Record){.type = CSV_Type_String, .length = token.length, .c_str = token.c_str});
                break;
            case CSV_TokenType_Number:
                csv_append(&csv_inst, (CSV_Record){.type = CSV_Type_Number, .length = token.length, .c_str = token.c_str});
                break;
            case CSV_TokenType_Bool:
                csv_append(&csv_inst, (CSV_Record){.type = CSV_Type_Bool, .length = token.length, .c_str = token.c_str});
                break;
            case CSV_TokenType_NULL:
                csv_append(&csv_inst, (CSV_Record){.type = CSV_Type_Null});
                break;
            default:
                return csv_inst;
        }

        skip_delimiter(&lexer);
    }

    return csv_inst;
}


size_t csv_rows(CSV * self) {
    return self->row;
}


size_t csv_columns(CSV * self) {
    return self->size / self->row;
}


static void * csv_iterator_next(Iterator * it) {
    if(it->index < ((CSV*) it->context)->size) {
        return &((CSV*) it->context)->record[it->index];
    } else {
        return NULL;
    }
}


static void csv_iterator_reset(Iterator * it) {
    it->index = 0;
}


Iterator csv_to_iterator(CSV * csv) {
    return (Iterator) {
        .context = csv
        , .__reset__ = csv_iterator_reset
        , .__next__ = csv_iterator_next
    };
}


CSV_Record * csv_at(CSV * self, size_t row, size_t column) {
    return &self->record[(row * (self->row / self->size)) + column];
}


void csv_show(CSV * self, char delimiter, FILE * stream) {
    size_t columns = csv_columns(self);
    iterate(csv_to_iterator(self), CSV_Record*, record, {
        if((iterator.index % columns) == (columns - 1)) {
            fprintf(stream, "%c %.*s\n", delimiter, (int) record->length, record->c_str);
        } else if ((iterator.index % columns) == 0) {
            fprintf(stream, "%.*s", (int) record->length, record->c_str);
        } else {
            fprintf(stream, "%c %.*s", delimiter, (int) record->length, record->c_str);
        } 
    });
}



void csv_finalize(CSV * self) {
    if(self->alloc != NULL && self->record != NULL) {
        delete(self->alloc, self->record);
    }
}






