#include "utility.h"
#include "lexer.h"

//extern FILE * fin;
//extern int token;
//extern int line;

int main(int argc, char ** argv) {

	// vector_words_traverse(&wordTable);

	fin = fopen("C:\\Users\\Rain\\hello.c", "rb");
	if (!fin) {
		printf("Error: Open file failed.\n");
		exit(-1);
	}

	
	init_lex();

	getch();

	do {
		next();
		code_coloring(LEX_NORMAL);

	} while (token != TOKEN_EOF);
	return 0;
}