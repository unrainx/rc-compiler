#ifndef RCC_UTILITY_H__
#define RCC_UTILITY_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>

#include "lexer.h"

#define true 1

#define false 0

// CC ADT

typedef struct Word {
	int tag;
	struct Word * next;
	char * spelling;
	struct Symbol * sym_struct;
	struct Symbol * sym_identifier;
} Word;

// String ADT
typedef struct String {
	int length;
	int capacity;
	char * text;
} String;

String tkstr;
String sourcestr;

void string_constructor(String * str, size_t size);

void string_destructor(String * str);

void string_reset(String * str);

void string_resize(String * str, size_t size);
	
void string_chcat(String * str, int ch);


// Vector ADT
typedef struct Vector {
	int length;
	int capacity;
	void ** data;
} Vector;

void vector_resize(Vector * vec, size_t size);

void vector_push(Vector * vec, void * data);

void vector_constructor(Vector * vec, size_t size);

void vector_destructor(Vector * vec);

int vector_find(Vector * vec, int key);


// Hash ADT

#define MAX_SIZE 1024
Word * wordHashTable[MAX_SIZE];
Vector wordTable;

int hash(char * key);

Word * word_reserve(Word * word);

Word * word_find(char * spelling);

Word * word_insert(char * spelling);

void * mallocz(int size);

void vector_words_traverse(Vector * vec);


enum ErrorLevel {
	ERROR_LEVEL_WARNING,
	ERROR_LEVEL_ERROR,
};


enum ErrorWorkStage {
	ERROR_WORK_STAGE_COMPILE,
	ERROR_WORK_STAGE_LINK,
};

void handle_exception(int stage, int level, char * format, va_list ap);

void warning(char * format, ...);

void error(char * format, ...);

void expect(char * message);

#endif // !RCC_UTILITY_H__