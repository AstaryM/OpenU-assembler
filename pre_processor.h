/*functions for the pre processor*/

#ifndef PRE_PROCESSOR
#define PRE_PROCESSOR

#include "definitions.h"
#include "macro_table.h"

/* just the pre-processing process (assuming macros don't have errors)*/
void pre_processor(FILE *source_file, FILE *new_file);

#endif