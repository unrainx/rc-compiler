#include "parser.h"

void translation_unit() {
	while (token != TOKEN_EOF) {
		external_declaration(SC_GLOBAL);
	}
}

void external_declaration(int l) {
	if (!type_specifier()) {
		expect("<type modifier>");
	}

	if (token == TOKEN_SEMICOLON) {
		next();
		return;
	}
	
	while (1) {
		declarator();
		if (token == TOKEN_BEGIN) {
			if (l == SC_LOCAL) {
				error("<Does not support function nesting>");
			}
			parse_funcbody();
			break
		} else {
			if (token == TOKEN_ASSIGN) {
				next();
				initializer();
			}

			if (token == TOKEN_COMMA) {
				next();
			} else {
				syntax_state = SNTX_LF_HT;
				skip(TOKEN_SEMICOLON);
				break;
			}
		}
	}
}

int type_specifier() {
	int type_found = 0;

	switch (token) {
	case KEYWORD_CHAR:
	case KEYWORD_SHORT:
	case KEYWORD_VOID:
	case KEYWORD_INT:
		type_found = 1;
		syntax_state = SNTX_SP;
		next();
		break;

	case KEYWORD_STRUCT:
		syntax_state = SNTX_SP;
		struct_specifier();
		type_found = 1;
		break;
	default:
		break;
	}

	return type_found;
}

void struct_specifier() {
	int v;
	next();
	v = token;
	syntax_state = SNTX_DELAY;
	next();

	if (token == TOKEN_BEGIN) {
		syntax_state = SNTX_LF_HF;
	} else if (token == TOKEN_CLOSEPAREN) {
		// sizeof(struct name)
		syntax_state = SNTX_NUL;
	} else {
		syntax_state = SNTX_SP;
	}
	syntax_indent();

	if (v < TOKEN_IDENT) {
		// 如果不是一个变量标识符
		expect("<struct name>");
	}
	if (token == TOKEN_BEGIN) {
		struct_declaration_list();
	}
}

void struct_declaration_list() {
	int maxalign, offset;
	syntax_state = SNTX_LF_HT;
	syntax_level++;

	next();
	while (token != TOKEN_END) {
		struct_declaration(&maxalign, &offset);
	}
	skip(TOKEN_END);
	syntax_state = SNTX_LF_HT;
}

void struct_declaration() {
	type_specifier();
	while (1) {
		declarator();

		if (token == TOKEN_SEMICOLON) {
			break;
		}
		skip(TOKEN_COMMA);
	}
	syntax_state = SNTX_LF_HT;
	skip(TOKEN_SEMICOLON);
}

void func_calling_convention(int * func) {
	*func = KEYWORD_CDECL;
	if (token == KEYWORD_CDECL || token == KEYWORD_STDCALL) {
		*func = token;
		syntax_state = SNTX_SP;
		next();
	}
}

// struct alignment
void struct_alignment() {
	if (token == KEYWORD_ALIGN) {
		next();
		skip(TOKEN_OPENPAREN);
		if (token == TOKEN_CONSTANT_INT) {
			next();
		} else {
			expect("<constant int>");
		}
		skip(TOKEN_CLOSEPAREN);
	}
}

void declarator() {
	int func;
	while (token == TOKEN_STAR) {
		next();
	}
	func_calling_convention(&func);
	struct_alignment();
	direct_declarator();
}

void direct_declarator() {
	if (token >= TOKEN_IDENT) {
		next();
	} else {
		expect("identifer");
	}
	direct_declarator_postfix();
}

void direct_declarator_postfix() {
	int n;
	// () (...) {} {cint}
	if (token == TOKEN_OPENPAREN) {
		parameter_type_list();
	} else if (token == TOKEN_OPENBR) {
		next();
		if (token == TOKEN_CONSTANT_INT) {
			next();
			n = tkvalue;
		}
		skip(TOKEN_CLOSEBR);
		direct_declarator_postfix();
	}
}

void parameter_type_list() {
	next();
	while (true)
	{
		if (!type_specifier()) {
			error("invalid identifier");
		}
		declarator();
		if (token == TOKEN_CLOSEPAREN) {
			break;
		}

		skip(TOKEN_COMMA);

		if (token == TOKEN_ELLIPSIS) {
			func_call = KEYWORD_CDECL;
			next();
			break;
		}
	}

	syntax_state = SNTX_DELAY;
	skip(TOKEN_CLOSEPAREN);

	if (token == TOKEN_BEGIN) {
		syntax_state = SNTX_LF_HT;
	} else {
		syntax_state = SNTX_NULL;
	}

	syntax_indent();
}

void funcbody() {
	compound_statement();
}

void initializer() {
	assignment_expression();
}

void statement(int * bsym, int csym) {
	switch (token)
	{
	case TOKEN_BEGIN:
		compound_statement();
		break;
	case KEYWORD_IF:
		if_statement();
		break;
	case KEYWORD_RETURN:
		return_statement();
		break;
	case KEYWORD_BREAK:
		break_statement();
		break;
	case KEYWORD_CONTINUE:
		continue_statement();
		break;
	case KEYWORD_FOR:
		for_statement();
		break;
	default:
		expression_statement();
		break;
	}
}

void compound_statement() {
	syntax_state = SNTX_LF_HT;
	syntax_level++;
	
	next();
	while (is_type_specifier(token)) {
		external_declaration(SC_LOCAL);
	}

	while (token != TOKEN_END) {
		statement();
	}

	syntax_state = SNTX_LF_HT;

	next();
}

int is_type_specifier(int v) {
	switch (v)
	{
	case KEYWORD_CHAR:
	case KEYWORD_SHORT:
	case KEYWORD_INT:
	case KEYWORD_VOID:
	case KEYWORD_STRUCT:
		return 1;
	default:
		break;
	}
	return 0;
}

void expression_statement() {
	if (token != TOKEN_SEMICOLON) {
		expression();
	}
	syntax_state = SNTX_LF_HT;
	next(TOKEN_SEMICOLON);
}

void if_statement() {
	syntax_state = SNTX_SP;
	next();
	skip(TOKEN_OPENPAREN);

	expression();

	syntax_state = SNTX_LF_HT;

	skip(TOKEN_CLOSEPAREN);
	
	statement();

	if (token == KEYWORD_ELSE) {
		syntax_state = SNTX_LF_HT;
		next();
		statement();
	}
}

void for_statement() {
	next();
	skip(TOKEN_OPENPAREN);

	if (token != TOKEN_SEMICOLON) {
		expression();
	}

	skip(TOKEN_SEMICOLON);

	if (token != TOKEN_SEMICOLON) {
		expression();
	}

	skip(TOKEN_SEMICOLON);

	if (token != TOKEN_CLOSEPAREN) {
		expression();
	}

	syntax_state = SNTX_LF_HT;

	skip(TOKEN_CLOSEPAREN);

	statement();
}


// <continue;>
void continue_statement() {
	next();
	syntax_state = SNTX_LF_HT;

	skip(TOKEN_SEMICOLON);
}

void break_statement() {
	next();
	syntax_state = SNTX_LF_HT;
	skip(TOKEN_SEMICOLON);
}

void return_statement() {
	syntax_state = SNTX_DELAY;
	next();
	if (token == TOKEN_SEMICOLON) {
		syntax_state = SNTX_NUL;
	} else {
		syntax_state = SNTX_SP;
	}
	syntax_indent();

	if (token != TOKEN_SEMICOLON) {
		expression();
	}

	syntax_state = SNTX_LF_HT;

	skip(TOKEN_SEMICOLON);
}