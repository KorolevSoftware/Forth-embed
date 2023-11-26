#include "forth_embed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "iso646.h"

#define array_size(x) sizeof(x)/sizeof(x[0])

enum token_type {
	tt_dup,
	tt_drop,
	tt_swap,
	tt_over,
	tt_rot,
	tt_dot,
	tt_dotstring,
	tt_emit,
	tt_cr,

	tt_equal,
	tt_great,
	tt_less,
	tt_invert,
	tt_and,
	tt_or,

	tt_plus,
	tt_minus,
	tt_multip,
	tt_div,

	tt_if,
	tt_else,
	tt_then,
	tt_do,
	tt_loop,

	tt_at, //dereferencing @
	tt_setvalue, // !
	tt_setfunction, // :
	tt_string,
	tt_ident, // identificator
	tt_semicolon, // ;
	tt_value,
	tt_endof,
	tt_none,
};

struct token {
	enum token_type type;
	union {
		int integer;
		char* string;
		char* name;
	} data;
};

struct token key_word_by_name(const char* word, const char* token_name, enum token_type type) {
	if (strcmp(token_name, word) == 0)
		return (struct token) { .type = type };
	return (struct token) { .type = tt_none };
}

#define key_word_func_by(fname) key_word_##fname
#define make_key_word_func(name, token_name, token_type) struct token key_word_##name(const char* word){ \
return key_word_by_name(word, token_name, token_type);\
}\



struct token_type_pair {
	enum token_type type;
	struct token(*func_cmp)(const char*);
};

make_key_word_func(dup, "dup", tt_dup);
make_key_word_func(drop, "drop", tt_drop);
make_key_word_func(swap, "swap", tt_swap);
make_key_word_func(over, "over", tt_over);
make_key_word_func(rot, "rot", tt_rot);
make_key_word_func(dot, ".", tt_dot);
make_key_word_func(dotstr, ".\"", tt_dotstring);
make_key_word_func(emit, "emit", tt_emit);
make_key_word_func(cr, "cr", tt_cr);

make_key_word_func(equal, "=", tt_equal);
make_key_word_func(great, "<", tt_great);
make_key_word_func(less, ">", tt_less);
make_key_word_func(invert, "invert", tt_invert);
make_key_word_func(andsc, "and", tt_and);
make_key_word_func(orsc , "or", tt_or);

make_key_word_func(plus, "+", tt_plus);
make_key_word_func(minus, "-", tt_minus);
make_key_word_func(multip, "*", tt_multip);
make_key_word_func(divsc, "/", tt_div);

make_key_word_func(ifsc, "if", tt_if);
make_key_word_func(elsesc, "else", tt_else);
make_key_word_func(then, "then", tt_then);
make_key_word_func(dosc, "do", tt_do);
make_key_word_func(loop, "loop", tt_loop);

make_key_word_func(at, "@", tt_at);
make_key_word_func(setvalue, "!", tt_setvalue);
make_key_word_func(setfunction, ":", tt_setfunction);
make_key_word_func(semicolon, ";", tt_semicolon);

struct token key_word_func_by(identifier) (const char* word) {
	if (strchr(word, '"') or strchr(word, '\\')) {
		return (struct token) { .type = tt_none };
	}
	return (struct token) { .type = tt_ident, .data.name = strdup(word) };
}

struct token key_word_func_by(string) (const char* word) {
	int word_lenght = strlen(word);
	if (word[word_lenght - 1] != '"')
		return (struct token) { .type = tt_none };

	return (struct token) { .type = tt_string, .data.string = strdup(word) };
}

struct token key_word_func_by(integer) (const char* word) {
	char* enp_pos;
	int value = strtol(word, &enp_pos, 10);
	uintptr_t integer_lenght = enp_pos - word;
	if (integer_lenght == strlen(word))
		return (struct token) { .type = tt_value, .data.integer = value };
	return (struct token) { .type = tt_none };
}


struct token_type_pair key_words[] = {
	{tt_dup, key_word_func_by(dup)},
	{tt_drop, key_word_func_by(drop)},
	{tt_swap, key_word_func_by(swap)},
	{tt_over, key_word_func_by(over)},
	{tt_rot, key_word_func_by(rot)},
	{tt_dot, key_word_func_by(dot)},
	{tt_dotstring, key_word_func_by(dotstr)},
	{tt_emit, key_word_func_by(emit)},
	{tt_cr, key_word_func_by(cr)},

	{tt_equal, key_word_func_by(equal)},
	{tt_great, key_word_func_by(great)},
	{tt_less, key_word_func_by(less)},
	{tt_invert, key_word_func_by(invert)},
	{tt_and, key_word_func_by(andsc)},
	{tt_or, key_word_func_by(orsc)},

	{tt_plus, key_word_func_by(plus)},
	{tt_minus, key_word_func_by(minus)},
	{tt_multip, key_word_func_by(multip)},
	{tt_div, key_word_func_by(divsc)},

	{tt_if, key_word_func_by(ifsc)},
	{tt_else, key_word_func_by(elsesc)},
	{tt_then, key_word_func_by(then)},
	{tt_do, key_word_func_by(dosc)},
	{tt_loop, key_word_func_by(loop)},

	{tt_at, key_word_func_by(at)},
	{tt_setvalue, key_word_func_by(setvalue)},
	{tt_setfunction, key_word_func_by(setfunction)},
	{tt_semicolon, key_word_func_by(semicolon)},
	

	/// Must be last
	{tt_value, key_word_func_by(integer)},
	{tt_string, key_word_func_by(string)},
	{tt_ident, key_word_func_by(identifier)},
};

struct token* tokenizer(const char* stream) {
	char* stream_copy = strdup(stream);

	char* word = strtok(stream_copy, " ");

	struct token* tokens = calloc(100, sizeof(struct token));
	int token_count = 0;

	while (word != NULL) {

		for (int key = 0; key < array_size(key_words); key++) {
			const struct token_type_pair key_word = key_words[key];
			struct token new_token = key_word.func_cmp(word);

			if (new_token.type == tt_none)
				continue;
			
			tokens[token_count] = new_token;
			break;
		}
		token_count++;
		word = strtok(NULL, " ");
	}
	tokens[token_count] = (struct token){ .type = tt_endof };
	return tokens;
}


static int stack_data[100];
static int stack_top = 0;


void stack_push(int value) {
	stack_data[stack_top] = value;
	stack_top++;
}

int stack_pop() {
	stack_top--;
	return stack_data[stack_top];
}


static int return_stack[10];
static int return_stack_top = 0;


void return_stack_push(int value) {
	return_stack[return_stack_top] = value;
	return_stack_top++;
}

int return_stack_pop() {
	return_stack_top--;
	return return_stack[return_stack_top];
}

struct function {
	const char* name;
	bool is_native;
	union {
		int position;
		void(*func_native)(void);
	} body;
};


static struct function function_table[10];
static int function_table_count = 0;

// ------------------------- STACK OPERATION -------------------------

void dup_op() {
	int value = stack_pop();
	stack_push(value);
	stack_push(value);
}

void drop_op() {
	stack_pop();
}

void swap_op() {
	int value1 = stack_pop();
	int value2 = stack_pop();
	stack_push(value1);
	stack_push(value2);
}

void over_op() {
	int value1 = stack_pop();
	int value2 = stack_pop();
	stack_push(value2);
	stack_push(value1);
	stack_push(value2);
}

void rot_op() {
	int value1 = stack_pop();
	int value2 = stack_pop();
	int value3 = stack_pop();
	stack_push(value2);
	stack_push(value1);
	stack_push(value3);
}

void dot_op() {
	int value = stack_pop();
	printf("%d", value);
}

void emit_op() {
	char value = (char)stack_pop();
	printf("%c", value);
}

void cr_op() {
	printf("/n");
}

// ------------------------- BOOLEAN OPERATION -------------------------
void equal_op() {
	int value1 = stack_pop();
	int value2 = stack_pop();

	int result =  value2 == value1 ? -1 : 0;
	stack_push(result);
}

void great_op() {
	int value1 = stack_pop();
	int value2 = stack_pop();

	int result = value2 > value1 ? -1 : 0;
	stack_push(result);
}

void less_op() {
	int value1 = stack_pop();
	int value2 = stack_pop();

	int result = value2 < value1 ? -1 : 0;
	stack_push(result);
}

void invert_op() {
	int value = stack_pop();
	stack_push(~value);
}

void and_op() {
	int value1 = stack_pop();
	int value2 = stack_pop();

	if (value1 == -1 && value2 == -1) {
		stack_push(-1);
	} else {
		stack_push(0);
	}
}

void or_op() {
	int value1 = stack_pop();
	int value2 = stack_pop();

	if (value1 == -1 || value2 == -1) {
		stack_push(-1);
	} else {
		stack_push(0);
	}
}

// ------------------------- MATH OPERATION -------------------------

void plus_op () {
	int value1 = stack_pop();
	int value2 = stack_pop();
	stack_push(value1 + value2);
}

void minus_op() {
	int value1 = stack_pop();
	int value2 = stack_pop();
	stack_push(value2 - value1);
}

void multiplication_op() {
	int value1 = stack_pop();
	int value2 = stack_pop();
	stack_push(value2 * value1);
}

void dividing_op() {
	int value1 = stack_pop();
	int value2 = stack_pop();
	stack_push(value2 / value1);
}


// ------------------------- tokinizer -------------------------


int if_op(const struct token* stream, int position) {
	const struct token main_body = stream[position + 1];

	int cmp = stack_pop();

	if (cmp == -1) {
		return position;
	}

	int temp_position = position+1;
	int if_stack = 0;
	while (true) {
		if (stream[temp_position].type == tt_if) {
			if_stack++;
		}

		if (stream[temp_position].type == tt_then) {
			if (if_stack == 0) {
				return temp_position;
			} else {
				if_stack--;
			}
		}
		if (stream[temp_position].type == tt_else) {
			if (if_stack == 0) {
				return temp_position;
			}
		}
		temp_position++;
	}
}

int skip_else(const struct token* stream, int position) {
	int if_stack = 0;
	int temp_position = position + 1;
	while (true) {
		if (stream[temp_position].type == tt_if) {
			if_stack++;
		}

		if (stream[temp_position].type == tt_then) {
			if (if_stack == 0) {
				return temp_position;
			} else {
				if_stack--;
			}
		}
		if (stream[temp_position].type == tt_else) {
			if (if_stack == 0) {
				return temp_position;
			}
		}
		temp_position++;
	}
}

int skip_function_body(const struct token* stream, int body_start_position) { //search ; (tt_semicolon) and return next position
	for (; stream[body_start_position].type != tt_semicolon; body_start_position++);
	return body_start_position; // skip tt_semicolon token
}

int set_function(const struct token* stream, int position) {
	const struct token name_token = stream[position + 1];
	function_table[function_table_count] = (struct function){ .is_native = 0, .name = name_token.data.name, .body.position = position + 1 };
	function_table_count++;
	return skip_function_body(stream, position);
}

int cull_function(const char* name) {
	for (int i = 0; i < function_table_count; i++) {
		struct function current = function_table[i];
		if (strcmp(current.name, name) == 0) {
			return current.body.position;
		}
	}
	return -1;
}

int do_loop(const struct token* stream, int position) {
	int start_index = stack_pop();
	int end_index = stack_pop();

	if (start_index < end_index) {
		return_stack_push(position -1);
		stack_push(end_index);
		stack_push(start_index + 1);
		return position;
	} else {
		int if_stack = 0;
		int temp_position = position + 1;
		while (true) {
			if (stream[temp_position].type == tt_do) {
				if_stack++;
			}

			if (stream[temp_position].type == tt_loop) {
				if (if_stack == 0) {
					return temp_position;
				} else {
					if_stack--;
				}
			}
			temp_position++;
		}
	}
}

void eval(const struct token* stream) {
	int current_pos = 0;

	for (int current_pos = 0; ; current_pos++) {
		const struct token current_token = stream[current_pos];
		enum token_type current_token_type = current_token.type;

		if (current_token_type == tt_value) {
			stack_push(current_token.data.integer);
		}

		if (current_token_type == tt_equal) {
			equal_op();
		}

		if (current_token_type == tt_plus) {
			plus_op();
		}

		if (current_token_type == tt_minus) {
			minus_op();
		}

		if (current_token_type == tt_div) {
			dividing_op();
		}

		if (current_token_type == tt_multip) {
			multiplication_op();
		}

		if (current_token_type == tt_dot) {
			dot_op();
		}

		if (current_token_type == tt_ident) {
			int jump_to_position;

			if (jump_to_position = cull_function(current_token.data.name)) { // cull function
				return_stack_push(current_pos);
				current_pos = jump_to_position;
			}
		}

		if (current_token_type == tt_if) {
			current_pos = if_op(stream, current_pos);
		}

		if (current_token_type == tt_else) {
			current_pos = skip_else(stream, current_pos);
		}

		if (current_token_type == tt_semicolon) {
			current_pos = return_stack_pop();
		}

		if (current_token_type == tt_setfunction) {
			current_pos = set_function(stream, current_pos);
		}

		if (current_token_type == tt_dotstring) {
			printf("%s", stream[current_pos+1].data.string);
			current_pos++;
		}

		if (current_token_type == tt_do) {
			current_pos = do_loop(stream, current_pos);
		}

		if (current_token_type == tt_loop) {
			current_pos = return_stack_pop();
		}

		if (current_token_type == tt_endof) {
			break;
		}
	}
}

struct forth_byte_code {
	struct token* stream;
};

const struct forth_byte_code* compile(const char* script) {
	struct forth_byte_code* byte_code = (struct forth_byte_code*)malloc(sizeof(struct forth_byte_code));
	byte_code->stream = tokenizer(script);
	return byte_code;
}


void run(struct forth_byte_code* script) {
	eval(script->stream);
}