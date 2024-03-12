#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "first_pass.h"
#include "definitions.h"
#include "line_processing.h"
#include "machine_word.h"
#include "utils.h"
#include "symbol_table.h"

/*the first pass line processing function*/
bool first_pass(line_ob line, symbol_table *t, symbol_table *potential_symbol_table, symbol_table *entries_table, int *IC, int *DC, long *code_image, long *data_image)
{
    char label_name[80] = "";

    /*if the line is to be ignored*/
    if (is_ignore(line))
    {
        return TRUE;
    }

    if (is_label(line))
    {
        substr(label_name, line.words[0], 0, strlen(line.words[0]) - 2);
        line.offset = 1;
        if (get_symbol_table_entry(*t, label_name) != NULL)
        {
            fprintf(stdout, "error: label is already defined! in line: %d\n", line.number);
            return FALSE;
        }
    }

    /*if the line is an instruction (הנחיה)*/
    if (is_instruction(line))
    {
        /*if the line contain a data instruction*/
        if (is_data(line))
        {
            if (line.offset)
                add_to_symbol_table(t, label_name, *DC, data, line.number);
            return process_data(line, DC, data_image);
        }

        /*if the line contain a string instruction*/
        if (is_string(line))
        {
            if (line.offset)
                add_to_symbol_table(t, label_name, *DC, data, line.number);
            return process_string(line, DC, data_image);
        } else/*if the line contain an external of entry instruction (or error)*/

        {
            return process_label_instruction(line, t, entries_table, DC);
        }

    }

    /*if the line is a command (הוראה)*/
    if (is_command(line))
    {
        if (line.offset)
            add_to_symbol_table(t, label_name, *IC, code, line.number);
        return process_code(line, potential_symbol_table, IC, code_image);
    }

    /*if the line is of unknown type*/
    fprintf(stdout, "error: unknown line type (not ignore, instruction or command). in line: %d\n", line.number);
    return FALSE;

}

/*for processing data instructions*/
bool process_data(line_ob line, int *DC, long *data_image)
{
    int i, value;
    for (i = 1 + line.offset; i < line.number_of_words; i++)
    {
        /*if one of the arguments is not int*/
        if (!is_str_num(line.words[i]))
        {

            fprintf(stdout, "error: incompatible type for data instruction (not int). in line: %d\n",
                    line.number);
            return FALSE;
        }


        value = atoi(line.words[i]);
        /*if the argument's value is beyond the range of the valid values*/
        if (value > MAX_DATA_VAL || value < MIN_DATA_VAL)
        {
            fprintf(stdout, "error: incompatible type for data instruction (out of range). in line: %d\n",
                    line.number);
            return FALSE;
        }
        data_image[(*DC)++] = get_data_word(value, A);
    }
    return TRUE;
}


/*for processing string instructions*/
bool process_string(line_ob line, int *DC, long *data_image)
{
    int j = 1, i;
    char curr_char;
    for (i = 1 + line.offset; i < line.number_of_words - 1 || j < strlen(line.words[i]) - 1; j++)
    {
        curr_char = line.words[i][j];
        /*checking if the argument is a graphical char as needed*/
        if (isgraph(curr_char))
            data_image[(*DC)++] = get_data_word(curr_char, A);
        else
        {
            fprintf(stdout, "error: incompatible type for string instruction (not graphical char). in line: %d\n",
                    line.number);
            return FALSE;
        }
        /*if we reached the end of the line*/
        if (j == strlen(line.words[i]) - 1)
        {

            j = 0;
            i++;
        }
    }
    data_image[(*DC)++] = get_data_word('\0', A);
    return TRUE;
}

/*for processing label related (entry or extern) instruction and dealing with errors*/
bool process_label_instruction(line_ob line, symbol_table *t, symbol_table *entries_table, int *DC)
{
    /*the name of the symbol will always be the second word in the line*/
    char *symbol_name = line.words[1];

    /*if is entry instruction, we will add it to the entries table, for the second pass*/
    if (is_entry(line))
    {
        add_to_symbol_table(entries_table, symbol_name, 0, entry, line.number);
        return TRUE;
    }

    /*if is extern instruction we will add it to the symbol table with the external attribute*/
    if (is_external(line))
    {
        add_to_symbol_table(t, symbol_name, 0, external, line.number);
        return TRUE;
    }

    /*if is error instruction*/
    fprintf(stdout, "error: unknown instruction (not data, string, entry or external). in line: %d\n",
            line.number);
    return FALSE;

}

/*for processing command lines (הוראות)*/
bool process_code(line_ob curr_line, symbol_table *potential_symbol_table, int *IC, long *code_image)
{
    op_ob operation = get_op(curr_line);
    int num_of_operands = curr_line.number_of_words - 1 - curr_line.offset;
    adress_modes first_add_mode = NO_ADD;
    adress_modes second_add_mode = NO_ADD;
    registers source_reg = no_r;
    registers dest_reg = no_r;
    long val;

    /*too many operands!*/
    if (num_of_operands > operation.number_of_operands)
    {
        fprintf(stdout, "error: too many operands! in line: %d\n",
                curr_line.number);
        return FALSE;
    }

    /*not enough operands!*/
    if (num_of_operands < operation.number_of_operands)
    {
        fprintf(stdout, "error: not enough operands! in line: %d\n",
                curr_line.number);
        return FALSE;
    }
    /*get the first machine word of the line (the one containing the opcode)*/
    code_image[(*IC)++] = get_opcode_word(operation);

    /*if the operation has operands we will build the second and perhaps the additional machine words*/
    if (num_of_operands > 0)
    {
        second_add_mode = get_ad_mode(curr_line, operation, num_of_operands, 2);

        if (second_add_mode == RE_ADD || second_add_mode == IN_ADD)
            dest_reg = get_operand_first_pass(curr_line, second_add_mode, num_of_operands);

        /*if there are two operands*/
        if (num_of_operands == 2)
        {
            first_add_mode = get_ad_mode(curr_line, operation, 1, 1);
            if (first_add_mode == RE_ADD || first_add_mode == IN_ADD)
                source_reg = get_operand_first_pass(curr_line, first_add_mode, 1);
        }

        /*if there are incompatible operands-error*/
        if (second_add_mode == ER_ADD || first_add_mode == ER_ADD)
        {
            fprintf(stdout,
                    "error: incompatible or unknown address mode. in line: %d\n",
                    curr_line.number);
            return FALSE;
        }

        /*we can now build the second machine word*/
        code_image[(*IC)++] = get_funct_word(operation, source_reg, first_add_mode, dest_reg, second_add_mode);

        /*getting the additional mahcine words*/

        /*for the first operand*/

        /*if the first operand has an additional machine word that will be added in the second pass*/
        if (num_of_operands == 2)
        {
            if (first_add_mode == DI_ADD || first_add_mode == IN_ADD)
            {
                /*we will add the label to the potential symbol table which wil be reviewed in the second pass*/
                get_label_operand(curr_line, potential_symbol_table, *IC, first_add_mode, 1);
                *IC += 2;
            }

            /*if the first operand is in immediate address, we can already build its additional machine word*/
            if (first_add_mode == IM_ADD)
            {
                if ((val = get_operand_first_pass(curr_line, IM_ADD, 1)) < MAX_DATA_VAL)
                    code_image[(*IC)++] = get_data_word(val, A);
            }

        }

        /*for the second operand*/

        /*if the second operand has an additional machine word that will be added in the second pass*/
        if (second_add_mode == DI_ADD || second_add_mode == IN_ADD)
        {
            /*we will add the label to the potential symbol table which wil be reviewed in the second pass*/
            get_label_operand(curr_line, potential_symbol_table, *IC, second_add_mode, num_of_operands);
            *IC += 2;
        }

        /*if the second operand is in immediate address, we can already build its additional machine word*/
        if (second_add_mode == IM_ADD)
        {

            if ((val = get_operand_first_pass(curr_line, IM_ADD, num_of_operands)) < MAX_DATA_VAL)
                code_image[(*IC)++] = get_data_word(val, A);
        }
    }
    return TRUE;
}
