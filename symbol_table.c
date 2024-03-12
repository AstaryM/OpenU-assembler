#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "symbol_table.h"
#include "utils.h"
#include "definitions.h"

/*adds an entry to the table*/
void add_to_symbol_table(symbol_table *t, char *symbol, int value,
                         attribute att, int line_number)
{
    /*creating the entry*/
    char *new_symbol;


    symbol_table entry;
    /*allocating new memory for the entry and its fields*/
    entry = (symbol_table) verified_malloc(sizeof(symbol_table_entry));
    new_symbol = (char *) verified_malloc(strlen(symbol) + 1);

    /*initializing the entry's fields*/
    strcpy(new_symbol, symbol);
    entry->symbol_name = new_symbol;
    entry->symbol_value = value;
    entry->base = (value / 16) * 16;
    entry->offset = value - entry->base;
    entry->attributes = att;
    entry->end = entry;
    entry->line_number = line_number;
    entry->next = NULL;


    /*if the table is empty the new entry will be its head*/
    if ((*t) == NULL)
        (*t) = entry;
    /*if not, we will add the entry to the end of the table*/
    else
    {
        ((*t)->end)->next = entry;
        ((*t)->end) = entry;
    }
}

/*returns a pointer to an entry with the same name*/
symbol_table get_symbol_table_entry(symbol_table t, char *symbol)
{
    symbol_table curr = t;
    while (curr != NULL)
    {
        /*if the table entry has the desired name we return a pointer to the entry*/
        if (!strcmp(curr->symbol_name, symbol))
            return (curr);
        curr = curr->next;
    }
    /*if not, we return error value*/
    return NULL;

}

/*gets a symbol value by name from the table*/
int get_symbol_value(symbol_table t, char *symbol)
{
    symbol_table curr = t;
    while (curr != NULL)
    {
        /*if the table entry has the desired name we return its value*/
        if (!strcmp(curr->symbol_name, symbol))
            return (curr->symbol_value);
        curr = curr->next;
    }

    /*if not, we return error value*/
    return -1;
}

/*increase the value of all entries on the table who have a specific attribute by a given amount*/
void add_value_to_type(symbol_table t, int value, attribute type)
{
    symbol_table curr = t;
    while (curr != NULL)
    {
        if (curr->attributes == type)
        {
            curr->symbol_value += value;
            curr->base = (curr->symbol_value / 16) * 16;
            curr->offset = curr->symbol_value - curr->base;
        }
        curr = curr->next;
    }
}

/*add the entry attribute to a specific entry by name*/
bool add_entry_attribute(symbol_table t, char *symbol_name)
{
    symbol_table curr = t;
    while (curr != NULL)
    {
        /*if successful*/
        if (!strcmp(symbol_name, curr->symbol_name))
        {
            if (curr->attributes == data)
            {
                curr->attributes = data_entry;
                return TRUE;
            }
            if (curr->attributes == code)
            {
                curr->attributes = code_entry;
                return TRUE;
            }

        }

        curr = curr->next;
    }

    /*if not successful return error value*/
    return FALSE;
}

/*free the table to prevent memory leaking*/
void free_symbol_table(symbol_table t)
{
    symbol_table prev, curr = t;
    while (curr != NULL)
    {
        prev = curr;
        curr = curr->next;
        /*freeing the allocated symbol names*/
        free(prev->symbol_name);
        /*freeing the allocated symbols*/
        free(prev);
    }
}






