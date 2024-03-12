/*functions for working with macro tables*/

#ifndef MACRO_TABLE
#define MACRO_TABLE

typedef struct macro_table_entry *macro_table;

/*macro table definitions*/
typedef struct macro_table_entry
{
    macro_table next;
    macro_table end;
    char *macro_name;
    char *macro_content;
} macro_table_entry;

/*adds an entry to the table*/
void add_to_macro_table(macro_table *t, char *macro_name);

/*returns macro content by name*/
void add_macro_content(macro_table t, char *new_content);

/*adds more lines to the content of the last macro in the table*/
char *get_macro_content(macro_table t, char *macro_name);

/*frees the macro table*/
void free_macro_table(macro_table t);

#endif