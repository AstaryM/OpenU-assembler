/*functions for the second pass*/
#ifndef SECOND_PASS
#define SECOND_PASS

#include "definitions.h"
#include "symbol_table.h"
#include "machine_word.h"


/*second pass function. updates the symbol table with entries symbols and updates the code image untouched additional machine words*/
bool second_pass(long *code_image, symbol_table *t, symbol_table *potential_symbol_table, symbol_table *entries_table, symbol_table *external_table);

/*updates the symbol-table's entries that were declared as entries to have the entry attribute*/
bool update_symbol_table(symbol_table *t, symbol_table *entries_table);

/*update the code image additional machine words that need label values*/
bool update_code_image(long *code_image, symbol_table *t, symbol_table *potential_symbol_table, symbol_table *externals_table);
#endif