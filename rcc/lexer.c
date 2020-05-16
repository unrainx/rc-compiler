#include "lexer.h"

void init_lex() {

	Word * word;
	static Word keywords[] = {
		{ TOKEN_PLUS,           NULL,        "+",     NULL,        NULL },
		{ TOKEN_MINUS,          NULL,        "=",     NULL,        NULL },
		{ TOKEN_STAR,           NULL,        "*",     NULL,        NULL },
		{ TOKEN_DIVIDE,         NULL,        "/",     NULL,        NULL },
		{ TOKEN_MOD,            NULL,        "%",     NULL,        NULL },
		{ TOKEN_EQ,             NULL,        "==",    NULL,        NULL },
		{ TOKEN_NEQ,            NULL,        "!=",    NULL,        NULL },
		{ TOKEN_LT,             NULL,        "<",     NULL,        NULL },
		{ TOKEN_LEQ,            NULL,        "<=",    NULL,        NULL },
		{ TOKEN_GT,             NULL,        ">",     NULL,        NULL },
		{ TOKEN_GEQ,            NULL,        ">=",    NULL,        NULL },
		{ TOKEN_ASSIGN,         NULL,        "=",     NULL,        NULL },
		{ TOKEN_POINTSTO,       NULL,        "->",    NULL,        NULL },
		{ TOKEN_DOT,            NULL,        ".",     NULL,        NULL },
		{ TOKEN_AND,            NULL,        "&",     NULL,        NULL },
		{ TOKEN_OPENPAREN,      NULL,        "(",     NULL,        NULL },
		{ TOKEN_CLOSEPAREN,     NULL,        ")",     NULL,        NULL },
		{ TOKEN_OPENBR,         NULL,        "[",     NULL,        NULL },
		{ TOKEN_CLOSEBR,        NULL,        "]",     NULL,        NULL },
		{ TOKEN_BEGIN,          NULL,        "{",     NULL,        NULL },
		{ TOKEN_END,            NULL,        "}",     NULL,        NULL },
		{ TOKEN_SEMICOLON,      NULL,        ";",     NULL,        NULL },
		{ TOKEN_COMMA,          NULL,        ",",     NULL,        NULL },
		{ TOKEN_ELLIPSIS,       NULL,        "...",     NULL,        NULL },
		{ TOKEN_EOF,            NULL,        "endOfFile",     NULL,        NULL },
		{ TOKEN_CONSTANT_INT,   NULL,        "const int",     NULL,        NULL },
		{ TOKEN_CONSTANT_CHAR,  NULL,        "const char",     NULL,        NULL },
		{ TOKEN_CONSTANT_STR,   NULL,        "const string",     NULL,        NULL },
		{ KEYWORD_CHAR,         NULL,        "char",     NULL,        NULL },
		{ KEYWORD_SHORT,        NULL,        "short",     NULL,        NULL },
		{ KEYWORD_INT,          NULL,        "int",     NULL,        NULL },
		{ KEYWORD_VOID,         NULL,        "void",     NULL,        NULL },
		{ KEYWORD_STRUCT,       NULL,        "struct",     NULL,        NULL },
		{ KEYWORD_IF,           NULL,        "if",     NULL,        NULL },
		{ KEYWORD_ELSE,         NULL,        "else",     NULL,        NULL },
		{ KEYWORD_FOR,          NULL,        "for",     NULL,        NULL },
		{ KEYWORD_CONTINUE,     NULL,        "continue",     NULL,        NULL },
		{ KEYWORD_BREAK,        NULL,        "break",     NULL,        NULL },
		{ KEYWORD_RETURN,       NULL,        "return",     NULL,        NULL },
		{ KEYWORD_SIZEOF,       NULL,        "sizeof",     NULL,        NULL },
		{ KEYWORD_ALIGN,        NULL,        "__align",     NULL,        NULL },
		{ KEYWORD_CDECL,        NULL,        "__cdecl",     NULL,        NULL },
		{ KEYWORD_STDCALL,      NULL,        "__stdcall",     NULL,        NULL },
		{ 0             ,       NULL, "", NULL, NULL}
	};

	vector_constructor(&wordTable, 8);
	for (word = &keywords[0]; word->spelling != NULL; word++) {
		word_reserve(word);
	}
}

void next() {
	preprocess();
	if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_') {
		
		parse_identifier();
		Word * word = word_insert(tkstr.text);
		//printf("<%s:%d>", word->spelling, word->tag);
		token = word->tag;
		return;
	} else if (ch >= '0' && ch <= '9') {
		parse_number();
		token = TOKEN_CONSTANT_INT;
		return;
	} else if (ch == '+') {
		getch();
		token = TOKEN_PLUS;
		return;
	} else if (ch == '-') {
		getch();
		if (ch == '>') {
			token = TOKEN_POINTSTO;
			getch();
		} else {
			token = TOKEN_MINUS;
		}
		return;
	} else if (ch == '/') {
		token = TOKEN_DIVIDE;
		getch();
		return;
	} else if (ch == '%') {
		token = TOKEN_MOD;
		getch();
		return;
	} else if (ch == '=') {
		getch();
		if (ch == '=') {
			token = TOKEN_EQ;
			getch();
		} else {
			token = TOKEN_ASSIGN;
		}
		return;
	} else if (ch == '!') {
		getch();
		if (ch == '=') {
			token = TOKEN_NEQ;
			getch();
		} else {
			error("! not support");
		}
		return;
	} else if (ch == '<') {
		getch();
		if (ch == '=') {
			token = TOKEN_LEQ;
			getch();
		} else {
			token = TOKEN_LT;
		}
		return;
	} else if (ch == '>') {
		getch();
		if (ch == '=') {
			token = TOKEN_GEQ;
			getch();
		} else {
			token = TOKEN_GT;
		}
		return;
	} else if (ch == '.') {
		getch();
		if (ch == '.') {
			getch();
			if (ch != '.') {
				error(".. spelling error");
			} else {
				token = TOKEN_ELLIPSIS;
			}
			getch();
		} else {
			token = TOKEN_DOT;
		}
	} else if (ch == '&') {
		token = TOKEN_AND;
		getch();
		return;
	} else if (ch == ';') {
		token = TOKEN_SEMICOLON;
		getch();
		return;
	} else if (ch == ']') {
		token = TOKEN_CLOSEBR;
		getch();
		return;
	} else if (ch == '[') {
		token = TOKEN_OPENBR;
		getch();
		return;
	} else if (ch == '(') {
		token = TOKEN_OPENPAREN;
		getch();
		return;
	} else if (ch == ')') {
		token = TOKEN_CLOSEPAREN;
		getch();
		return;
	} else if (ch == '{') {
		token = TOKEN_BEGIN;
		getch();
		return;
	} else if (ch == '}') {
		token = TOKEN_END;
		getch();
		return;
	} else if (ch == ',') {
		token = TOKEN_COMMA;
		getch();
		return;
	} else if (ch == '*') {
		token = TOKEN_STAR;
		getch();
		return;
	} else if (ch == '\'') {
		parse_string(ch);
		token = TOKEN_CONSTANT_CHAR;
		tkvalue = * (char *)tkstr.text;
		return;
	} else if (ch == '\"') {
		parse_string(ch);
		token = TOKEN_CONSTANT_STR;
		return;
	} else if (ch == EOF) {
		token = TOKEN_EOF;
	} else {
		error("unexpected character");
		getch();
	}
}

void getch() {
	ch = getc(fin);
	//printf("read <%c>", ch);
}

void preprocess() {

	while (true) {
		if (ch == ' ' || ch == '\t' || ch == '\r') {
			skip_white_space();
		} else if (ch == '/') {
			getch();
			if (ch == '*') {
				parse_comment();
			} else {
				ungetc(ch, fin);
				ch = '/';
				break;
			}
		} else {
			break;
		}
	}
}

void parse_comment() {
	getch();
	do {
		do {
			if (ch == '\n' || ch == '*' || ch == EOF) {
				break;
			} else {
				getch();
			}
		} while (true);

		if (ch == '\n') {
			line += 1;
			getch();
		} else if (ch == '*') {
			getch();
			if (ch == '/') {
				getch();
				return;
			}
		} else {
			error("unexpected character");
			return;
		}
	} while (true);
}

void skip_white_space() {
	while (ch == ' ' || ch == '\t' || ch == '\r') {
		if (ch == '\r') {
			getch();
			if (ch != '\n') {
				return;
			}
			line += 1;
		}
		printf("%c", ch);
		getch();
	}
}

int is_nodigit(char c) {
	return (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_');
}

int is_digit(char c) {
	return c >= '0' && c <= '9';
}

void parse_identifier() {
	string_reset(&tkstr);
	string_chcat(&tkstr, ch);
	getch();
	while (is_nodigit(ch) || is_digit(ch)) {
		string_chcat(&tkstr, ch);
		getch();
	}
	string_chcat(&tkstr, '\0');
}

void parse_number() {
	string_reset(&tkstr);
	string_reset(&sourcestr);

	do {
		string_chcat(&tkstr, ch);
		string_chcat(&sourcestr, ch);
		getch();
	} while (is_digit(ch));

	if (ch == '.') {
		do {
			string_chcat(&tkstr, ch);
			string_chcat(&sourcestr, ch);
			getch();
		} while (is_digit(ch));
	}

	string_chcat(&tkstr, '\0');
	string_chcat(&sourcestr, '\0');
	tkvalue = atoi(tkstr.text);
}

void parse_string(char sep) {
	char c;
	string_reset(&tkstr);
	string_reset(&sourcestr);
	string_chcat(&sourcestr, sep);
	getch();

	for (;;) {
		if (ch == sep) {
			break;
		} else if (ch == '\\') {
			string_chcat(&sourcestr, ch);
			getch();
			switch (ch)
			{
			case '0':
				c = '\0';
				break;
			case 'a':
				c = '\a';
				break;
			case 'b':
				c = '\b';
				break;
			case 't':
				c = '\t';
				break;
			case 'n':
				c = '\n';
				break;
			case 'v':
				c = '\v';
			case 'f':
				c = '\f';
			case 'r':
				c = '\r';
				break;
			case '\"':
				c = '\"';
				break;
			case '\'':
				c = '\'';
				break;
			case '\\':
				c = '\\';
				break;
			default:
				warning("unexpected character");
				break;
			}

			string_chcat(&tkstr, c);
			string_chcat(&sourcestr, ch);
			getch();
		} else {
			string_chcat(&tkstr, ch);
			string_chcat(&sourcestr, ch);
			getch();
		}
	}
	string_chcat(&tkstr, '\0');
	string_chcat(&sourcestr, sep);
	string_chcat(&sourcestr, '\0');
	getch();
}

void code_coloring(int lex_state) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	char * p;
	switch (lex_state) {
	case LEX_NORMAL:
		if (token == TOKEN_EOF) {
			SetConsoleTextAttribute(handle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			return;
		} else if (token >= TOKEN_IDENT) {
			SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
		} else if (token >= KEYWORD_CHAR) {
			SetConsoleTextAttribute(handle, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		} else if (token >= TOKEN_CONSTANT_INT) {
			SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN);
		} else {
			SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		p = get_tkstr(token);
		printf("%s", p);
		break;
	case LEX_SEP:
		printf("%c", ch);
		break;
	default:
		break;
	}
}

char * get_tkstr(int v) {
	if (v > wordTable.length) {
		return NULL;
	} else if (v >= TOKEN_CONSTANT_INT && v <= TOKEN_CONSTANT_STR) {
		return sourcestr.text;
	} else {
		return ((Word *)wordTable.data[v])->spelling;
	}
}