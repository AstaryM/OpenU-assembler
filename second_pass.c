#include <stddef.h>
#include <stdio.h>
#include "second_pass.h"
#include "symbol_table.h"
#include "definitions.h"

/*second pass function. updates the symbol table with entries symbols and updates the code image untouched additional machine words*/
bool
second_pass(long *code_image, symbol_table *t, symbol_table *potential_symbol_table, symbol_table *entries_table, symbol_table *externals_table)
{
    return update_symbol_table(t, entries_table) &&
           update_code_image(code_image, t, potential_symbol_table, externals_table);
}

/*updates the symbol-table's entries that were declared as entries to have the entry attribute*/
bool update_symbol_table(symbol_table *t, symbol_table *entries_table)
{
    symbol_table curr = *entries_table;
    /*going over the entries table from the first pass*/
    while (curr != NULL)
    {

        /*if the label name does not exist (and updating the entry attribute)*/
        if (!add_entry_attribute(*t, curr->symbol_name))
        {
            fprintf(stdout, "error: entry label does not exist. in line: %d\n", curr->line_number);
            return FALSE;

        }

        /*adjusting the symbol value at the entries table*/
        curr->symbol_value = get_symbol_value(*t, curr->symbol_name);
        curr->base = (curr->symbol_value / 16) * 16;
        curr->offset = curr->symbol_value - curr->base;

        curr = curr->next;
    }
    return TRUE;

}

/*update the code image additional machine words that need label values*/
bool
update_code_image(long *code_image, symbol_table *t, symbol_table *potential_symbol_table, symbol_table *externals_table)
{
    symbol_table curr = *potential_symbol_table, temp;
    ARE type;
    while (curr != NULL)
    {
        /*if the label exists*/
        if ((temp = get_symbol_table_entry(*t, curr->symbol_name)) != NULL)
        {
            type = R;
            /*if the label is external we will add it to the externals table*/
            if (temp->attributes == external)
            {
                add_to_symbol_table(externals_table, temp->symbol_name, curr->symbol_value, external,
                                    curr->line_number);
                type = E;
            }

            /*completing the code image*/
            code_image[curr->symbol_value] = get_data_word(temp->base, type);
            code_image[curr->symbol_value + 1] = get_data_word(temp->offset, type);


        } else/*the label does not exists-error*/
        {
            fprintf(stdout, "error: label used as operand does not exist. in line: %d\n", curr->line_number);
            return FALSE;
        }

        curr = curr->next;
    }
    return TRUE;
}
