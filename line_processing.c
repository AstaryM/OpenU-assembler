#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"
#include "symbol_table.h"
#include "definitions.h"
#include "line_processing.h"

/*returns whether the line is to be ignored*/
bool is_ignore(line_ob line)
{
    return (line.number_of_words == 0 || line.str[0] == ';');
}

/*returns whether the line contain a label*/
bool is_label(line_ob line)
{
    return (line.words[0][strlen(*line.words) - 1] == ':' && is_legal_label(line.words[0], 1));
}

/*returns whether the line contain an instruction*/
bool is_instruction(line_ob line)
{
    return (line.words[line.offset][0] == '.');
}

/*returns whether the line contain a command*/
bool is_command(line_ob line)
{
    int i;
    for (i = 0; i < 16; i++)
        if (!strcmp(op_table[i].name, line.words[line.offset]))
            return TRUE;
    return FALSE;
}

/*returns whether the instruction is data*/
bool is_data(line_ob line)
{
    return !strcmp(line.words[line.offset], ".data");
}

/*returns whether the instruction is string*/
bool is_string(line_ob line)
{
    return ((!strcmp(line.words[line.offset], ".string")) &&
            (line.words[line.number_of_words - 1][strlen(line.words[line.number_of_words - 1]) - 1] = '\"') &&
            (line.words[1 + line.offset][0] == '\"'));
}

/*returns whether the instruction is entry*/
bool is_entry(line_ob line)
{
    return (!strcmp(line.words[0], ".entry") && (line.number_of_words == 2) && (is_legal_label(line.words[1], 0)));
}

/*returns whether the instruction is extern*/
bool is_external(line_ob line)
{
    return (!strcmp(line.words[0], ".extern") && (line.number_of_words == 2) && is_legal_label(line.words[1], 0));
}

/*returns the operation of the command*/
op_ob get_op(line_ob line)
{
    int i;
    char *name = line.words[line.offset];
    /*checks for the operation in the lookup table*/
    for (i = 0; i < 16; i++)
    {
        if (!strcmp(name, op_table[i].name))
            return op_table[i];
    }
    return op_table[NUM_OF_OPERATIONS];
}

/*returns the first or second (operand_pos) operand's address mode*/
adress_modes get_ad_mode(line_ob line, op_ob operation, int operand_pos, int operand_num)
{
    /*string of the first or second (operand_pos) operand*/
    char *operand = line.words[1 + line.offset + operand_pos - 1];
    /*checking and returning*/
    if (is_immediate_add(operand))
    {
        if (is_valid_add_mode(IM_ADD, operation, operand_num))
            return IM_ADD;
    }
    if (is_direct_add(operand))
    {
        if (is_valid_add_mode(DI_ADD, operation, operand_num))
            return DI_ADD;
    }
    if (is_index_add(operand))
    {

        if (is_valid_add_mode(IN_ADD, operation, operand_num))
            return IN_ADD;
    }
    if (is_register_add(operand))
    {
        if (is_valid_add_mode(RE_ADD, operation, operand_num))
            return RE_ADD;
    }
    /*none of the above-error*/
    return ER_ADD;
}

/*returns whether the address mode is immediate*/
bool is_immediate_add(char *operand)
{
    return (operand[0] == '#' && is_str_num(operand + 1));
}

/*returns whether the address mode is direct*/
bool is_direct_add(char *operand)
{
    return is_legal_label(operand, 0);
}

/*returns whether the address mode is index*/
bool is_index_add(char *operand)
{
    int length = strlen(operand);
    return (operand[length - 1] == ']' && is_legal_label(operand, 5) && operand[length - 5] == '[' &&
            operand[length - 4] == 'r' && operand[length - 3] == '1' && isdigit(operand[length - 2]) &&
            operand[length - 2] < '6');
}

/*returns whether the address mode is direct register*/
bool is_register_add(char *operand)
{
    return (operand[0] == 'r' && ((strlen(operand) == 2 && isdigit(operand[1])) || (
            (strlen(operand) == 3) && operand[1] == '1' && isdigit(operand[2]) &&
            operand[2] < '6')));
}

/*returns whether the address mode is valid with the operation*/
bool is_valid_add_mode(adress_modes mode, op_ob operation, int operand_num)
{
    /*if it's the first operand*/
    if (operand_num == 1)
    {
        return (operation.first_address_modes[mode]);
    }
    /*if it's the second operand*/
    if (operand_num == 2)
    {
        return (operation.second_address_modes[mode]);
    }
    return FALSE;
}

/*returns the value of the first or second operand (operand_pos) which will be used in the first pass*/
int get_operand_first_pass(line_ob line, adress_modes m, int operand_pos)
{

    int pos = line.offset + operand_pos;

    if (m == IM_ADD)
    {
        char *operand_str;
        operand_str = line.words[pos] + 1;
        return atoi(operand_str);
    }

    if (m == IN_ADD)
    {
        char *temp, operand_str[80];
        temp = line.words[pos] + strlen(line.words[pos]) - 3;
        strncpy(operand_str, temp, 2);
        return atoi(operand_str);
    }

    if (m == RE_ADD)
    {
        char *operand_str;
        operand_str = line.words[pos] + 1;
        return (atoi(operand_str));
    }
    return -1;
}

/*gets the names of labels used as operands and adds them to the potential symbol table*/
void get_label_operand(line_ob line, symbol_table *potential_symbol_table, int IC, adress_modes m, int operand_pos)
{
    int pos = line.offset + operand_pos;
    /*if the operand's addressing mode is direct address*/
    if (m == DI_ADD)
    {
        char *operand_str;
        operand_str = line.words[pos];
        add_to_symbol_table(potential_symbol_table, operand_str, IC, potential, line.number);
    }
    /*if the operand's addressing mode is immediate address*/
    if (m == IN_ADD)
    {
        char operand_str[80];
	memset(operand_str, 0, 80);
        strncpy(operand_str, line.words[pos], strlen(line.words[pos]) - 5);
        add_to_symbol_table(potential_symbol_table, operand_str, IC, potential, line.number);

    }

}
