#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "macro_table.h"

/*adds an entry to the table*/
void add_to_macro_table(macro_table *t, char *macro_name)
{
    /*initialization*/
    char *new_macro_name, *new_macro_content;
    macro_table entry;
    entry = (macro_table) verified_malloc(sizeof(macro_table_entry));
    new_macro_name = (char *) verified_malloc(strlen(macro_name) + 1);
    new_macro_content = (char *) verified_malloc(1);

    /*assigning values*/
    strcpy(new_macro_name, macro_name);
    new_macro_content[0] = '\0';

    /*adding the values to the entry fields*/
    entry->macro_name = new_macro_name;
    entry->macro_content = new_macro_content;
    entry->end = entry;
    entry->next = NULL;

    /*adding the entry to the table*/
    if ((*t) == NULL)
    {
        (*t) = entry;
    } else
    {
        ((*t)->end)->next = entry;
        (*t)->end = entry;
    }
}

/*returns macro content by name*/
char *get_macro_content(macro_table t, char *macro_name)
{
    macro_table curr = t;
    while (curr != NULL)
    {
        if (!strcmp(curr->macro_name, macro_name))
            return (curr->macro_content);
        curr = curr->next;
    }
    return NULL;
}

/*adds more lines to the content of the last macro in the table*/
void add_macro_content(macro_table t, char *new_content)
{
    macro_table curr = t->end;
    /*resizing the last entry*/
    curr->macro_content = (char *) realloc(curr->macro_content, strlen(curr->macro_content) + strlen(new_content) + 1);
    strcat((curr)->macro_content, new_content);

}


/*frees the macro table*/
void free_macro_table(macro_table t)
{
    macro_table prev, curr = t;
    while (curr != NULL)
    {
        prev = curr;
        curr = curr->next;
        free(prev->macro_name);
        free(prev->macro_content);
        free(prev);
    }
}
