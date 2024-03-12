/*contains definitions and globals*/

#ifndef DEFINITIONS
#define DEFINITIONS

/*general values from the booklet*/
#define MAX_DATA_VAL 16384
#define MIN_DATA_VAL -16384
#define MAX_MACHINE_CODE_LENGTH 8192
#define MAX_LINE_LENGTH 81
#define MAX_WORDS_IN_LINE 40/*there are at max 40 words in a line of length 80 (every word is of length 1 and there are spaces*/
#define IC_BASE_VAL 100
#define MAX_LABEL_LENGTH 31

/*positions of the parts of the machine word*/
#define ARE_POS 16
#define DEST_REG_POS 2
#define SOURCE_ADD_POS 6
#define SOURCE_REG_POS 8
#define FUNCT_POS 12

/*tables sizes*/
#define NUM_OF_OPERATIONS 16
#define NUM_OF_RESERVED_WORDS 35

/*ARE's values*/
typedef enum ARE
{
    A = 4,
    R = 2,
    E = 1
} ARE;

/*opcode's values*/
typedef enum opcodes
{
    ER_OP = 0,

    MOV_OP = 1,
    CMP_OP = 2,
    ADD_OP = 4,
    SUB_OP = 4,
    LEA_OP = 16,
    CLR_OP = 32,
    NOT_OP = 32,
    INC_OP = 32,
    DEC_OP = 32,
    JMP_OP = 512,
    BNE_OP = 512,
    JSR_OP = 512,
    RED_OP = 4096,
    PRN_OP = 8192,
    RTS_OP = 16384,
    STOP_OP = 32768
} opcodes;

/*funct's values*/
typedef enum functs
{
    ER_FUNCT = -1,
    NO_FUNCT = 0,
    ADD_FUNCT = 10,
    SUB_FUNCT = 11,
    CLR_FUNCT = 10,
    NOT_FUNCT = 11,
    INC_FUNCT = 12,
    DEC_FUNCT = 13,
    JMP_FUNCT = 10,
    BNE_FUNCT = 11,
    JSR_FUNCT = 12
} functs;

/*adresse mode's values*/
typedef enum adress_modes
{
    ER_ADD = -1,
    IM_ADD = 0,
    DI_ADD = 1,
    IN_ADD = 2,
    RE_ADD = 3,
    NO_ADD = 0

} adress_modes;

/*operation object containing all of the needed information*/
typedef struct op_ob
{
    char *name;
    opcodes code;
    functs funct;
    int first_address_modes[5];
    int second_address_modes[5];
    int number_of_operands;
} op_ob;

/*the operation's lookup table*/
static struct op_ob op_table[] = {
        {"mov",   MOV_OP,  NO_FUNCT,  {1, 1, 1, 1, 0}, {0, 1, 1, 1, 0}, 2},
        {"cmp",   CMP_OP,  NO_FUNCT,  {1, 1, 1, 1, 0}, {1, 1, 1, 1, 0}, 2},
        {"add",   ADD_OP,  ADD_FUNCT, {1, 1, 1, 1, 0}, {0, 1, 1, 1, 0}, 2},
        {"sub",   SUB_OP,  SUB_FUNCT, {1, 1, 1, 1, 0}, {0, 1, 1, 1, 0}, 2},
        {"lea",   LEA_OP,  NO_FUNCT,  {0, 1, 1, 0, 0}, {0, 1, 1, 1, 0}, 2},
        {"clr",   CLR_OP,  CLR_FUNCT, {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, 1},
        {"not",   NOT_OP,  NOT_FUNCT, {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, 1},
        {"inc",   INC_OP,  INC_FUNCT, {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, 1},
        {"dec",   DEC_OP,  DEC_FUNCT, {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, 1},
        {"jmp",   JMP_OP,  JMP_FUNCT, {0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, 1},
        {"bne",   BNE_OP,  BNE_FUNCT, {0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, 1},
        {"jsr",   JSR_OP,  JSR_FUNCT, {0, 0, 0, 0, 0}, {0, 1, 1, 0, 0}, 1},
        {"red",   RED_OP,  NO_FUNCT,  {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, 1},
        {"prn",   PRN_OP,  NO_FUNCT,  {0, 0, 0, 0, 0}, {1, 1, 1, 1, 0}, 1},
        {"rts",   RTS_OP,  NO_FUNCT,  {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, 0},
        {"stop",  STOP_OP, NO_FUNCT,  {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, 0},
        {"error", ER_OP,   ER_FUNCT,  {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, 0}};

/*register's values*/
typedef enum registers
{
    no_r = 0,
    r0 = 0,
    r1 = 1,
    r2 = 2,
    r3 = 3,
    r4 = 4,
    r5 = 5,
    r6 = 6,
    r7 = 7,
    r8 = 8,
    r9 = 9,
    r10 = 10,
    r11 = 11,
    r12 = 12,
    r13 = 13,
    r14 = 14,
    r15 = 15

} registers;

/*reserved words lookup table*/
static char *reserved_list[] = {
        "mov",
        "cmp",
        "add",
        "sub",
        "lea",
        "clr",
        "not",
        "inc",
        "dec",
        "jmp",
        "bne",
        "jsr",
        "red",
        "prn",
        "rts",
        "stop",
        "r0",
        "r1",
        "r2",
        "r3",
        "r4",
        "r5",
        "r6",
        "r7",
        "r8",
        "r9",
        "r10",
        "r11",
        "r12",
        "r13",
        "r14",
        "r15",
        "data",
        "string",
        "extern",
        "entry"

};

/*line object containing useful information*/
typedef struct line_ob
{
    char str[MAX_LINE_LENGTH];/*the basic string to the line*/
    char **words;/*array containing all of the line's words*/
    int number;/*the number of the line in the file*/
    int number_of_words;/*number of words in the line*/
    int offset;/*whether the line contain a label declaration*/
} line_ob;

/*basic boolean type*/
typedef enum
{
    FALSE,
    TRUE
} bool;

#endif

