#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "definitions.h"
#include "macro_table.h"
#include "pre_processor.h"

/* just the pre-processing process (assuming macros don't have errors)*/
void pre_processor(FILE *source_file, FILE *dest_file)
{
    macro_table t = NULL;
    char curr_line[MAX_LINE_LENGTH];
    char macro_name[MAX_LINE_LENGTH];
    char first_word[MAX_LINE_LENGTH];
    bool macro_flag = FALSE;
    char *macro_content;


    while (!feof(source_file))
    {
        fgets(curr_line, 80, source_file);
        get_nth_word(first_word, curr_line, 1);

        /*turning off the macro flag*/
        if (!strcmp(first_word, "endm"))
        {
            macro_flag = FALSE;
            continue;
        }

        /*turning on the macro flag*/
        if (!strcmp(first_word, "macro"))
        {
            macro_flag = TRUE;
            get_nth_word(macro_name, curr_line, 2);
            add_to_macro_table(&t, macro_name);
            continue;
        }

        /*if currently in macro, we will update the macro content*/
        if (macro_flag)
            add_macro_content(t, curr_line);



        /*if currently not in macro we will print to the dest file*/
        if (!macro_flag)
        {

            macro_content = get_macro_content(t, first_word);
            /*if the line has a macro call, we will print the macro content*/
            if (macro_content != NULL)
            {
                fprintf(dest_file, "%s", macro_content);
            } else/*if not, we will print the line itself*/
            {
                fprintf(dest_file, "%s", curr_line);
            }
        }
    }

    free_macro_table(t);
}


