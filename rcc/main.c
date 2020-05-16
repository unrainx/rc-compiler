#include "utility.h"
#include "lexer.h"


int main(int argc, char ** argv) {

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