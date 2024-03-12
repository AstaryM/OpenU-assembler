#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include "definitions.h"
#include "utils.h"

/*just a malloc with verification*/
void *verified_malloc(int size)
{
    /*malloc*/
    void *temp = malloc(size);
    /*verification*/
    if (temp == NULL)
    {
        printf("memory allocation error");
        exit(1);
    }

    return temp;
}

/*makes dest into a substring from "start" index to "end" index of str*/
void substr(char *dest, char *str, int start, int end)
{
    int i;
    for (i = 0; i < end + 1 && i + start < strlen(str); i++)
        dest[i] = str[i + start];
    dest[end - start + 1] = '\0';
}

/*breaks a line into an array of words*/
int break_line(char *list[], char *str)
{
    int i, temp, j = 0;
    for (i = 0; i < strlen(str); i++)
    {
        temp = 0;
        while (isspace(str[i]))
            i++;
        if (str[i] != '\0' && !isspace(str[i]))
        {
            if (list[j] == NULL)
                list[j] = (char *) verified_malloc(MAX_LINE_LENGTH);
            while (str[i] != '\0' && !isspace(str[i]))
                list[j][temp++] = str[i++];
            list[j][temp] = '\0';
            j++;
        }
    }
    return j;
}

/*delete all the legal commas in a line object and returns whether the commas placement is valid. the new value of num_of_words is the new number of words after deleting the commas*/
bool delete_commas(line_ob line, int *num_of_words)
{
    int seperated_comma_counter = 0;
    int comma_counter = 1;
    int curr_value;
    int i;
    /*if the number of words in line is less than 3 or line is string then there are no legal commas*/
    if (line.number_of_words - line.offset < 3 || !strcmp(line.words[line.offset], ".string"))
        return TRUE;
    /*the first operand cannot start with a comma*/
    if (line.words[1 + line.offset][0] != ',')
    {
        for (i = 1 + line.offset; i < *num_of_words; i++)
        {
            /*we "fill the holes" that were left after we removed seperated commas*/
            curr_value = i - seperated_comma_counter;
            strcpy(line.words[curr_value], line.words[i]);

            /*if the first char of the word is a comma*/
            if (line.words[curr_value][0] == ',')
            {
                /*if it is a "seperated comma" we increase the counter*/
                if (!strcmp(line.words[i], ","))
                    seperated_comma_counter++;

                comma_counter++;
                substr(line.words[curr_value], line.words[curr_value], 1, strlen(line.words[curr_value]) - 1);
            }

                /*if the first char is not a comma we decrease the counter*/
            else
                comma_counter--;

            /*error in comma placement*/
            if (comma_counter < 0 || comma_counter > 1)
                return FALSE;

            /*if the string is not empty we reset the counter*/
            if (strcmp(line.words[curr_value], ""))
                comma_counter = 0;

            /*if the last char of the string is a comma (not including seperated commas)*/
            if (line.words[curr_value][strlen(line.words[curr_value]) - 1] == ',')
            {
                comma_counter++;
                line.words[curr_value][strlen(line.words[curr_value]) - 1] = '\0';
            }

            /*"cleaning" the transferred words*/
            if (seperated_comma_counter)
                strcpy(line.words[i], "");
        }

        /*if the last comma is valid we can return TRUE*/
        if (!comma_counter)
        {
            /*decreasing the sentence length*/
            *num_of_words -= seperated_comma_counter;
            return TRUE;

        }
    }

    /*error*/
    return FALSE;
}

/*returns the nth word of a string*/
void get_nth_word(char *dest, char *line, int n)
{
    int i = 0, j = 0, in_word_flag = 0;

    while (n >= 0 && i < strlen(line))
    {
        /*getting to the next word*/
        while (isspace(line[i]))
        {
            in_word_flag = 0;
            i++;
        }

        /*if we reached a word*/
        while (!isspace(line[i]) && line[i] != '\n')
        {
            if (!in_word_flag)
                n--;
            in_word_flag = 1;
            /*if we reached the nth word*/
            if (!n)
            {
                dest[j++] = line[i];
            }
            i++;
        }
    }
    /*reducing the string's length*/
    dest[j] = '\0';
}

/*returns whether a substring of a string with length strlen-n is a reserved words*/
bool is_reserved(char *str, int n)
{
    int i, length = strlen(str) - n;
    for (i = 0; i < NUM_OF_RESERVED_WORDS; i++)
    {
        if (!strncmp(str, reserved_list[i], length))
            return TRUE;

    }
    return FALSE;
}

/*returns whether a substring of a string with length strlen-n is a legal label name*/
bool is_legal_label(char *str, int n)
{
    int i;
    if (isalpha(str[0]) && strlen(str) < MAX_LABEL_LENGTH && !is_reserved(str, n))
    {
        for (i = 0; i < strlen(str) - n; i++)
            if (!isalnum(str[i]))
                return FALSE;
        return TRUE;
    }
    return FALSE;
}

/*returns whether a string consists of numeric chars*/
bool is_str_num(char *str)
{
    int i;
    /*if there is a sign we will get to the next char*/
    if (str[0] == '-' || str[0] == '+')
        str++;

    for (i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]))
            return FALSE;
    }
    return TRUE;
}
