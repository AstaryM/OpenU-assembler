#include "machine_word.h"
#include "definitions.h"

/*returns the opcode machine word (the first word of a command line)*/
long get_opcode_word(op_ob operation)
{

    long new_machine_word = 0;
    new_machine_word += operation.code;
    new_machine_word += A << ARE_POS;
    return new_machine_word;
}

/*returns the funct machine word (the second word of a command line)*/
long
get_funct_word(op_ob operation, registers first_register, adress_modes first_add_mode, registers second_register, adress_modes second_add_mode)
{
    long new_machine_word = 0;
    new_machine_word += second_add_mode;
    new_machine_word += second_register << DEST_REG_POS;
    new_machine_word += first_add_mode << SOURCE_ADD_POS;
    new_machine_word += first_register << SOURCE_REG_POS;
    new_machine_word += operation.funct << FUNCT_POS;
    new_machine_word += A << ARE_POS;
    return new_machine_word;
}

/*returns a data type machine word*/
long get_data_word(int value, ARE t)
{
    long new_machine_word = 0;
    /*if the value is positive we can add it to the word*/
    if (value >= 0)
        new_machine_word += value;
    else/*if the value is negative we have to adjust it to two's complement 16 bit int*/
        new_machine_word += (value & NEGATIVE_MASK);
    new_machine_word += t << ARE_POS;
    return new_machine_word;
}


