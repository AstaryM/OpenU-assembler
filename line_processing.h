/*functions for processing a line object*/

#ifndef LINE_PROCESSING
#define LINE_PROCESSING

#include "definitions.h"
#include "symbol_table.h"

/*returns whether the line is to be ignored*/
bool is_ignore(line_ob line);

/*returns whether the line contain a label*/
bool is_label(line_ob line);

/*returns whether the line contain an instruction*/
bool is_instruction(line_ob line);

/*returns whether the line contain a command*/
bool is_command(line_ob line);

/*returns whether the instruction is data*/
bool is_data(line_ob line);

/*returns whether the instruction is string*/
bool is_string(line_ob line);

/*returns whether the instruction is entry*/
bool is_entry(line_ob line);

/*returns whether the instruction is extern*/
bool is_external(line_ob line);

/*returns whether the address mode is valid with the operation*/
bool is_valid_add_mode(adress_modes mode, op_ob operation, int operand_num);

/*returns the operation of the command*/
op_ob get_op(line_ob line);

/*returns the first or second (operand_pos) operand's address mode*/
adress_modes get_ad_mode(line_ob line, op_ob operation, int operand_pos, int operand_num);

/*returns whether the address mode is immediate*/
bool is_immediate_add(char *operand);

/*returns whether the address mode is direct*/
bool is_direct_add(char *operand);

/*returns whether the address mode is index*/
bool is_index_add(char *operand);

/*returns whether the address mode is direct register*/
bool is_register_add(char *operand);

/*returns the value of the first or second operand (operand_pos) which will be used in the first pass*/
int get_operand_first_pass(line_ob line, adress_modes m, int num);

/*gets the names of labels used as operands and adds them to the potential symbol table*/
void get_label_operand(line_ob line, symbol_table *potential_symbol_table, int IC, adress_modes m, int operand_pos);

#endif
