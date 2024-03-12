#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "symbol_table.h"
#include "second_pass.h"
#include "pre_processor.h"
#include "line_processing.h"
#include "first_pass.h"
#include "definitions.h"
#include "output_files.h"

bool assembler(char *file_name);

int main(int argc, char *argv[]) {
    int i;
    bool success_flag;
    for (i = 1; i < argc; i++) {
        success_flag = assembler(argv[i]);
        /*if there were errors in the file we will break the line*/
        if (!success_flag)
            puts("");
    }

    return 0;
}

bool assembler(char *file_name) {

    int file_name_length = strlen(file_name);
    char *as_name = NULL;
    char *am_name = NULL;
    char *ob_name = NULL;
    char *en_name = NULL;
    char *ex_name = NULL;
    FILE *as = NULL;
    FILE *am = NULL;
    FILE *ob = NULL;
    FILE *en = NULL;
    FILE *ex = NULL;
    int IC = IC_BASE_VAL, DC = 0, ICF, DCF, j;
    long code_image[MAX_MACHINE_CODE_LENGTH], data_image[MAX_MACHINE_CODE_LENGTH];/*images*/
    symbol_table t = NULL, potential_symbol_table = NULL, entry_table = NULL, external_table = NULL;/*tables*/
    char *temp_list[MAX_WORDS_IN_LINE];/*line.words initializer*/
    line_ob curr_line;
    bool success_flag = TRUE;

    /*allocating memory for the file names*/
    as_name = (char *) malloc(file_name_length + 4);
    am_name = (char *) malloc(file_name_length + 4);
    ob_name = (char *) malloc(file_name_length + 4);
    en_name = (char *) malloc(file_name_length + 5);
    ex_name = (char *) malloc(file_name_length + 5);

    /*initializing the file names with NULL*/
    memset(as_name, 0, file_name_length + 4);
    memset(am_name, 0, file_name_length + 4);
    memset(ob_name, 0, file_name_length + 4);
    memset(en_name, 0, file_name_length + 5);
    memset(ex_name, 0, file_name_length + 5);

    /*initializing the file names*/
    strcpy(as_name, file_name);
    strcpy(am_name, file_name);
    strcpy(ob_name, file_name);
    strcpy(en_name, file_name);
    strcpy(ex_name, file_name);

    /*opening the input files*/
    as = fopen(strcat(as_name, ".as"), "r");
    am = fopen(strcat(am_name, ".am"), "w+");

    /*if an error occurred in the opening*/
    if (as == NULL || am == NULL) {
        fprintf(stdout, "error in opening the files");
        return FALSE;
    }



    /*pre processor*/
    pre_processor(as, am);

    rewind(am);

    /*first pass*/

    /*initializing the words array of the line*/
    curr_line.words = temp_list;
    for (curr_line.number = 1; fgets(curr_line.str, MAX_LINE_LENGTH, am) != NULL; curr_line.number++) {
        /*initializing the line's fields*/
        curr_line.number_of_words = break_line(curr_line.words, curr_line.str);
        curr_line.offset = is_label(curr_line);

        /*if there are errors with the commas placement*/
        if (!delete_commas(curr_line, &curr_line.number_of_words)) {
            fprintf(stdout, "error: illegal placement of commas. in line: %d\n", curr_line.number);
            success_flag = FALSE;

        }

        /*analyzing the line*/
        success_flag &= first_pass(curr_line, &t, &potential_symbol_table, &entry_table, &IC, &DC, code_image,
                                   data_image);
    }

    /*post first pass*/

    /*freeing the allocated line memory*/
    for (j = 0; j < MAX_WORDS_IN_LINE; j++) {
        if (curr_line.words[j] != NULL)
            free(curr_line.words[j]);
        curr_line.words[j] = NULL;
    }
    ICF = IC;
    DCF = DC;
    add_value_to_type(t, ICF, data);

    /*second pass*/
    success_flag &= second_pass(code_image, &t, &potential_symbol_table, &entry_table, &external_table);


    /*if there are no errors we can print the files*/
    if (success_flag) {
        /*opening the output files*/
        ob = fopen(strcat(ob_name, ".ob"), "w");
        en = fopen(strcat(en_name, ".ent"), "w");
        ex = fopen(strcat(ex_name, ".ext"), "w");

        /*if an error occurred in the opening*/
        if (ob == NULL || en == NULL || ex == NULL) {
            fprintf(stdout, "error in opening files");
            return FALSE;
        }

        /*printing the output*/
        print_ob_file(ob, code_image, data_image, ICF, DCF);
        print_externals_file(ex, external_table);
        print_entries_file(en, entry_table);


        /*closing the output files*/
        fclose(ob);
        fclose(en);
        fclose(ex);
    }

    free(as_name);
    free(am_name);
    free(ob_name);
    free(en_name);
    free(ex_name);


    /*freeing the tables*/
    free_symbol_table(t);
    free_symbol_table(potential_symbol_table);
    free_symbol_table(entry_table);
    free_symbol_table(external_table);

    /*closing the files*/
    if (as != NULL) {
        fclose(as);
        as = NULL;
    }
    if (am != NULL) {
        fclose(am);
        am = NULL;
    }

    return success_flag;

}
