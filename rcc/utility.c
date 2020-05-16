#include "utility.h"

// String implement
extern int token;

void string_constructor(String * str, size_t size) {
	if (str) {
		str->text = (char *)malloc(sizeof(char) * size);
		str->length = 0;
		str->capacity = size;
	}
}

void string_destructor(String * str) {
	if (str) {
		if (str->text) { free(str->text); }
		str->length = 0;
		str->capacity = 0;
	}
}

void string_reset(String * str) {
	string_destructor(str);
	string_constructor(str, 32);
}

void string_resize(String * str, size_t size) {
	int capacity = 0;
	char * text = NULL;

	capacity = size * 1.5;
	text = realloc(str->text, capacity);
	if (!text) {
		printf("error: %s line %d Realloc memory failed", __FILEW__, __LINE__);
		exit(-1);
	}
	str->capacity = capacity;
	str->text = text;
}

void string_chcat(String * str, int ch) {
	if (str) {
		int length = str->length + 1;
		if (length > str->capacity) {
			string_resize(str, length);
		}
		(str->text)[length - 1] = ch;
		str->length = length;
	}
}


// implement Vector

void vector_resize(Vector * vec, size_t size) {
	int capacity = 0;
	void * data;

	capacity = vec->capacity;
	capacity = size * 1.5;
	data = realloc(vec->data, capacity);
	if (!data) {
		printf("error: %s line %d Realloc memory failed", __FILEW__, __LINE__);
		exit(-1);
	}
	vec->capacity = capacity;
	vec->data = data;
}

void vector_push(Vector * vec, void * data) {
	int length = vec->length + 1;
	if (length * sizeof(void *) > vec->capacity) {
		vector_resize(vec, length * sizeof(void *));
	}
	vec->data[length - 1] = data;
	vec->length = length;
}

void vector_constructor(Vector * vec, size_t size) {
	if (vec) {
		vec->data = (void **)malloc(sizeof(char) * size);
		vec->length = 0;
		vec->capacity = size;
	}
}

void vector_destructor(Vector * vec) {
	void ** data;
	for (data = vec->data; vec->length != 0; data++, vec->length--) {
		if (*data) {
			free(*data);
		}
	}
	free(vec->data);
	vec->data = NULL;
}

int vector_find(Vector * vec, int key) {
	int ** data;
	data = (int **)vec->data;
	for (int i = 0; i < vec->length; i++, data++) {
		if (key == **data) {
			return i;
		}
	}
	return -1;
}


// implement Hash

int elf_hash(char * key) {
	unsigned h = 0;
	char * c = key;
	while (*c != '\0') {
		h = (h << 5) + *c++;
	}
	return h % MAX_SIZE;
}

Word * word_reserve(Word * word) {
	int key;
	vector_push(&wordTable, word);
	key = elf_hash(word->spelling);

	while (wordHashTable[key] != NULL) {
		wordHashTable[key] = wordHashTable[key]->next;
	}
	wordHashTable[key] = word;
	word->next = NULL;

	//printf("\nreserve<%s: %d: hash-%d>\n", word->spelling, word->tag, key);
	
	return word;
}

Word * word_find(char * spelling) {
	Word * word = NULL;
	int key = elf_hash(spelling);
	for (Word * w = wordHashTable[key]; w != NULL; w = w->next) {
		if (!strcmp(spelling, w->spelling)) {
			int tag = w->tag;
			word = w;
			break;
		}
	}
	return word;
}

Word * word_insert(char * spelling) {
	Word * word;
	int key;
	char * s;
	char * end;
	int length;
	
	key = elf_hash(spelling);
	//printf("\ninsert: <%s: hash-%d>", spelling, key);
	word = word_find(spelling);

	if (word == NULL) {
		length = strlen(spelling);
		word = (Word *)malloc(sizeof(Word) + length + 1);		

		while (wordHashTable[key] != NULL) {
			wordHashTable[key] = wordHashTable[key]->next;
		}
		wordHashTable[key] = word;
		word->next = NULL;

		vector_push(&wordTable, word);

		// tag -> index
		word->tag = wordTable.length - 1;
		s = (char *)word + sizeof(Word);
		word->spelling = (char *)s;
		for (end = spelling + length; spelling < end;) {
			*s++ = *spelling++;
		}
		*s = '\0';
	}
	return word;
}

void * mallocz(int size) {
	void * ptr;
	ptr = malloc(size);
	if (!ptr && size) {
		printf("error: %s line %d Malloc memory failed", __FILEW__, __LINE__);
		exit(-1);
	}
	memset(ptr, 0, size);
	return ptr;
}

void vector_words_traverse(Vector * vec) {
	printf("\ninfo: This vector's length is %d\n", vec->length);
	printf("info: This vector's capacity is %d\n", vec->capacity);
	int len = vec->length;
	for (int i = 0; i < len; i++) {
		Word * w = (Word *)vec->data[i];
		printf(" %s \n", w->spelling);
	}
}

void handle_exception(int stage, int level, char * format, va_list ap) {
	char buf[1024];
	vsprintf(buf, format, ap);
	if (stage == ERROR_LEVEL_WARNING) {
#define filename __FILE__
#define _line __LINE__
		printf("%s<line %d>: warning: %s.\n", filename, _line, buf);
#undef filename
#undef _line	
	}
}

void warning(char * format, ...) {
	va_list ap;
	va_start(ap, format);
	// va_arg(ap, type)
	handle_exception(ERROR_WORK_STAGE_COMPILE, ERROR_LEVEL_WARNING, format, ap);
	va_end(ap);
}

void error(char * format, ...) {
	va_list ap;
	va_start(ap, format);
	handle_exception(ERROR_WORK_STAGE_COMPILE, ERROR_LEVEL_ERROR, format, ap);
	va_end(ap);
}

void expect(char * message) {
	error("lack %s", message);
}

void skip(int c) {
	if (token != c) {
		////error("lack '%s'", get_token_str(c));
	}
	////get_token();
}

//char * get_token_str(int v) {
//	if (v > wordTable.length) {
//		return NULL;
//	} else if (v >= TOKEN_CONSTANT_INT && v <= TOKEN_CONSTANT_STR) {
//		// return *(sourcestr.data);
//		return "fads";
//	} else {
//		return ((Word *)wordTable.data[v])->spelling;
//	}
//}

void link_error(char * format, ...) {
	va_list ap;
	va_start(ap, format);
	handle_exception(ERROR_WORK_STAGE_LINK, ERROR_LEVEL_ERROR, format, ap);
	va_end(ap);
}