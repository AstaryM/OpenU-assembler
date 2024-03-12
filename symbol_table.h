/*functions for working with a table data structure*/

#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include "definitions.h"

/*attributes of symbols*/
typedef enum
{
    external,
    entry,
    data,
    code,
    data_entry,
    code_entry,
    potential

} attribute;



/*definitions and functions for a symbol table, for storing the symbols names and values*/

/* defining the table as a pointer to its head*/
typedef struct symbol_table_entry *symbol_table;
/*defining the table's entries*/
typedef struct symbol_table_entry
{
    char *symbol_name;
    int symbol_value;
    int base;
    int offset;
    attribute attributes;
    symbol_table next;
    symbol_table end;
    int line_number;
} symbol_table_entry;


/*adds an entry to the table*/
void add_to_symbol_table(symbol_table *t, char *symbol, int value, attribute att, int line_number);

/*returns a pointer to an entry with the same name*/
symbol_table get_symbol_table_entry(symbol_table, char *symbol);

/*gets a symbol value by name from the table*/
int get_symbol_value(symbol_table t, char *symbol);

/*increase the value of all entries on the table who have a specific attribute by a given amount*/
void add_value_to_type(symbol_table t, int value, attribute type);

/*add the entry attribute to a specific entry by name*/
bool add_entry_attribute(symbol_table t, char *symbol_name);

/*free the table to prevent memory leaking*/
void free_symbol_table(symbol_table t);


#endif