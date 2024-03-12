/*functions for the output files*/
#ifndef OUTPUT_FILES
#define OUTPUT_FILES

#include <stdio.h>
#include "symbol_table.h"

/*values for bit maskings*/
#define A_MASK 0xf0000
#define B_MASK 0x0f000
#define C_MASK 0x00f00
#define D_MASK 0x000f0
#define E_MASK 0x0000f

/*values for bit shifting*/
#define A_SHIFT 16
#define B_SHIFT 12
#define C_SHIFT 8
#define D_SHIFT 4
#define E_SHIFT 0

/*makes the object file*/
void print_ob_file(FILE *ob_file, long *code_image, long *data_image, int ICF, int DCF);

/*makes the externals file*/
void print_externals_file(FILE *externals_file, symbol_table externals_table);

/*makes the entries file*/
void print_entries_file(FILE *entries_file, symbol_table entries_table);


#endif
