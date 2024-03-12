assembler: utils.o symbol_table.o second_pass.o pre_processor.o output_files.o macro_table.o machine_word.o line_processing.o first_pass.o main.o
	gcc -g -ansi -Wall -pedantic utils.o symbol_table.o second_pass.o pre_processor.o output_files.o macro_table.o machine_word.o line_processing.o first_pass.o main.o -o assembler
utils.o: utils.c definitions.h utils.h
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o
symbol_table.o: symbol_table.c symbol_table.h utils.h definitions.h
	gcc -c -ansi -Wall -pedantic symbol_table.c -o symbol_table.o
second_pass.o: second_pass.c second_pass.h symbol_table.h definitions.h
	gcc -g -c -ansi -Wall -pedantic second_pass.c -o second_pass.o
pre_processor.o: pre_processor.c pre_processor.h utils.h definitions.h macro_table.h
	gcc -c -ansi -Wall -pedantic pre_processor.c -o pre_processor.o
output_files.o: output_files.c output_files.h definitions.h symbol_table.h
	gcc -c -ansi -Wall -pedantic output_files.c -o output_files.o
macro_table.o: macro_table.c macro_table.h utils.h
	gcc -c -ansi -Wall -pedantic macro_table.c -o macro_table.o
machine_word.o: machine_word.c machine_word.h definitions.h
	gcc -c -ansi -Wall -pedantic machine_word.c -o machine_word.o
line_processing.o: line_processing.c line_processing.h utils.h symbol_table.h definitions.h
	gcc -g -c -ansi -Wall -pedantic line_processing.c -o line_processing.o
first_pass.o: first_pass.c first_pass.h definitions.h line_processing.h machine_word.h utils.h symbol_table.h
	gcc -c -ansi -Wall -pedantic first_pass.c -o first_pass.o
main.o: main.c output_files.h symbol_table.h second_pass.h pre_processor.h line_processing.h first_pass.h definitions.h
	gcc -g -c -ansi -Wall -pedantic main.c -o main.o
