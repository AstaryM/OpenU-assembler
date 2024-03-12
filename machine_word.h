/*functions for working with machine words*/

#ifndef MACHINE_WORD
#define MACHINE_WORD

#include "definitions.h"

#define NEGATIVE_MASK 0xffff

/*returns the opcode machine word (the first word of a command line)*/
long get_opcode_word(op_ob operation);

/*returns the funct machine word (the second word of a command line)*/
long
get_funct_word(op_ob operation, registers first_register, adress_modes first_add_mode, registers second_register, adress_modes second_add_mode);

/*returns a data type machine word*/
long get_data_word(int value, ARE t);

#endif
