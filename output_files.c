#include <stdio.h>
#include "output_files.h"
#include "definitions.h"
#include "symbol_table.h"

/*making the object file*/
void print_ob_file(FILE *ob_file, long *code_image, long *data_image, int ICF, int DCF)
{
    int i, a, b, c, d, e;
    fprintf(ob_file, "%d %d\n", ICF - 100, DCF);

    /*going over the code image*/
    for (i = IC_BASE_VAL; i < ICF; i++)
    {
        a = (code_image[i] & A_MASK) >> A_SHIFT;
        b = (code_image[i] & B_MASK) >> B_SHIFT;
        c = (code_image[i] & C_MASK) >> C_SHIFT;
        d = (code_image[i] & D_MASK) >> D_SHIFT;
        e = (code_image[i] & E_MASK) >> E_SHIFT;

        fprintf(ob_file, "%04d A%x-B%x-C%x-D%x-E%x\n", i, a, b, c, d, e);
    }

    /*going over the data image*/
    for (i = 0; i < DCF; i++)
    {
        a = (data_image[i] & A_MASK) >> A_SHIFT;
        b = (data_image[i] & B_MASK) >> B_SHIFT;
        c = (data_image[i] & C_MASK) >> C_SHIFT;
        d = (data_image[i] & D_MASK) >> D_SHIFT;
        e = (data_image[i] & E_MASK) >> E_SHIFT;

        fprintf(ob_file, "%04d A%x-B%x-C%x-D%x-E%x\n", i + ICF, a, b, c, d, e);
    }

}

/*making the externals file*/
void print_externals_file(FILE *externals_file, symbol_table externals_table)
{
    symbol_table curr = externals_table;
    while (curr != NULL)
    {
        fprintf(externals_file, "%s BASE %04d\n", curr->symbol_name, curr->symbol_value);
        fprintf(externals_file, "%s OFFSET %04d\n", curr->symbol_name, curr->symbol_value + 1);
        curr = curr->next;
    }
}

/*making the entries file*/
void print_entries_file(FILE *entries_file, symbol_table entries_table)
{
    symbol_table curr = entries_table;
    while (curr != NULL)
    {
        fprintf(entries_file, "%s, %04d,%04d\n", curr->symbol_name, curr->base, curr->offset);
        curr = curr->next;
    }
}
