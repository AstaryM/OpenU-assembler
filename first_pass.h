/*functions for the first pass*/

#ifndef FIRST_PASS
#define FIRST_PASS

#include "definitions.h"
#include "line_processing.h"
#include "machine_word.h"
#include "symbol_table.h"

/*the first pass line processing function*/
bool first_pass(line_ob line, symbol_table *t, symbol_table *potential_symbol_table, symbol_table *entries_table, int *IC, int *DC, long *code_image, long *data_image);

/*for processing data instructions*/
bool process_data(line_ob line, int *DC, long *data_image);

/*for processing string instructions*/
bool process_string(line_ob line, int *DC, long *data_image);

/*for processing label related (entry or extern) instruction and dealing with errors*/
bool process_label_instruction(line_ob line, symbol_table *t, symbol_table *entries_table, int *DC);

/*for processing command lines (הוראות)*/
bool process_code(line_ob curr_line, symbol_table *potential_symbol_table, int *IC, long *code_image);

#endif