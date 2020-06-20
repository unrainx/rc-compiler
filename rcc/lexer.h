#ifndef RCC_LEXER_H__
#define RCC_LEXER_H__

#include <windows.h>

#include "utility.h"

int token;
int line;
int tkvalue;
char ch;
FILE * fin;

enum Tag {

	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_STAR,
	TOKEN_DIVIDE,
	TOKEN_MOD,
	TOKEN_EQ,
	TOKEN_NEQ,
	TOKEN_LT,
	TOKEN_LEQ,
	TOKEN_GT,
	TOKEN_GEQ,
	TOKEN_ASSIGN, // =
	TOKEN_POINTSTO, // ->
	TOKEN_DOT, // .
	TOKEN_AND,
	TOKEN_OPENPAREN, // (
	TOKEN_CLOSEPAREN, // )
	TOKEN_OPENBR, // [
	TOKEN_CLOSEBR, // ]
	TOKEN_BEGIN, // {
	TOKEN_END, // }
	TOKEN_SEMICOLON, // ;
	TOKEN_COMMA, // ,
	TOKEN_ELLIPSIS, // ... 
	TOKEN_EOF, 


	TOKEN_CONSTANT_INT,
	TOKEN_CONSTANT_CHAR,
	TOKEN_CONSTANT_STR,

	// KeyWord
	KEYWORD_CHAR,
	KEYWORD_SHORT,
	KEYWORD_INT,
	KEYWORD_VOID,
	KEYWORD_STRUCT,
	KEYWORD_IF,
	KEYWORD_ELSE,
	KEYWORD_FOR,
	KEYWORD_CONTINUE,
	KEYWORD_BREAK,
	KEYWORD_RETURN,
	KEYWORD_SIZEOF,

	KEYWORD_ALIGN,
	KEYWORD_CDECL,
	KEYWORD_STDCALL,


	// ID
	TOKEN_IDENT,
};

enum LexerState {
	LEX_NORMAL,
	LEX_SEP,
};

void init_lex();

void next();

void getch();

void preprocess();

void parse_comment();

void skip_white_space();

int is_nodigit(char c);

int is_digit(char c);

void parse_identifier();

void parse_number();

void parse_string(char sep);

void code_coloring(int lex_state);

char * get_tkstr(int v);

#endif // !RCC_LEXER_H__
