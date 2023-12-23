#include "forth_embed.h"
#include <stdio.h>
#include <assert.h>

#define PASS() printf("Pass %s\n", __func__);

extern struct token;

// testing forth componets
void drop_op(struct forth_state* fs);
void swap_op(struct forth_state* fs);
void over_op(struct forth_state* fs);
void rot_op(struct forth_state* fs);

void dot_op(struct forth_state* fs);
void emit_op(struct forth_state* fs);
void cr_op();

int push_pop(void) {
	struct forth_state* fs = forth_make_default_state();

	forth_data_stack_push(fs, 1);
	forth_data_stack_push(fs, 2);
	forth_data_stack_push(fs, 3);
	forth_data_stack_push(fs, 4);

	int s4 = forth_data_stack_pop(fs);
	int s3 = forth_data_stack_pop(fs);
	int s2 = forth_data_stack_pop(fs);
	int s1 = forth_data_stack_pop(fs);

	forth_release_state(fs);

	assert(s4 == 4);
	assert(s3 == 3);
	assert(s2 == 2);
	assert(s1 == 1);

	PASS();
	return 0;
}

int drop() {
	struct forth_state* fs = forth_make_default_state();
	int result;

	forth_data_stack_push(fs, 1);
	forth_data_stack_push(fs, 2);
	forth_data_stack_push(fs, 3);

	// 1
	drop_op(fs); // test func
	int d2 = forth_data_stack_pop(fs);
	assert(d2 == 2);

	// 2
	forth_data_stack_push(fs, d2);

	drop_op(fs); // test func
	int d1 = forth_data_stack_pop(fs);
	assert(d1 == 1);


	forth_release_state(fs);
	PASS();
	return 0;
}

int swap() {
	struct forth_state* fs = forth_make_default_state();

	forth_data_stack_push(fs, 1);
	forth_data_stack_push(fs, 2);
	forth_data_stack_push(fs, 3);


	swap_op(fs);

	
	int d3 = forth_data_stack_pop(fs);
	int d2 = forth_data_stack_pop(fs);
	int d1 = forth_data_stack_pop(fs);

	assert(d3 == 2);
	assert(d2 == 3);
	assert(d1 == 1);


	forth_release_state(fs);
	PASS();

	return 0;
}

int over() {
	struct forth_state* fs = forth_make_default_state();

	forth_data_stack_push(fs, 1);
	forth_data_stack_push(fs, 2);
	forth_data_stack_push(fs, 3);


	over_op(fs);


	int d2 = forth_data_stack_pop(fs);
	int d3 = forth_data_stack_pop(fs);
	int dd2 = forth_data_stack_pop(fs);
	int d1 = forth_data_stack_pop(fs);

	assert(d2 == 2);
	assert(d3 == 3);
	assert(dd2 == 2);
	assert(d1 == 1);

	forth_release_state(fs);
	PASS();

	return 0;
}

int rot() {
	struct forth_state* fs = forth_make_default_state();

	forth_data_stack_push(fs, 1);
	forth_data_stack_push(fs, 2);
	forth_data_stack_push(fs, 3);


	rot_op(fs);


	int d1 = forth_data_stack_pop(fs);
	int d3 = forth_data_stack_pop(fs);
	int d2 = forth_data_stack_pop(fs);

	assert(d2 == 2);
	assert(d3 == 3);
	assert(d1 == 1);

	forth_release_state(fs);
	PASS();

	return 0;
}

int dot() {
	struct forth_state* fs = forth_make_default_state();

	forth_data_stack_push(fs, 1);
	forth_data_stack_push(fs, 2);
	forth_data_stack_push(fs, 3);

	printf("Dot operator must be print: %d %d %d\n", 3, 2, 1);

	dot_op(fs);
	dot_op(fs);
	dot_op(fs);
	
	cr_op(fs);
	forth_release_state(fs);
	PASS();

	return 0;
}

int emit() {
	struct forth_state* fs = forth_make_default_state();

	forth_data_stack_push(fs, 'd');
	forth_data_stack_push(fs, 'l');
	forth_data_stack_push(fs, 'r');
	forth_data_stack_push(fs, 'o');
	forth_data_stack_push(fs, 'w');
	forth_data_stack_push(fs, ' ');
	forth_data_stack_push(fs, 'o');
	forth_data_stack_push(fs, 'l');
	forth_data_stack_push(fs, 'l');
	forth_data_stack_push(fs, 'e');
	forth_data_stack_push(fs, 'h');

	printf("emit operator must be print: hello world\n");

	emit_op(fs);
	emit_op(fs);
	emit_op(fs);
	emit_op(fs);
	emit_op(fs);
	emit_op(fs);
	emit_op(fs);
	emit_op(fs);
	emit_op(fs);
	emit_op(fs);
	emit_op(fs);
	cr_op(fs);
	forth_release_state(fs);
	PASS();

	return 0;
}

int cr() {
	struct forth_state* fs = forth_make_default_state();
	printf("Start and new line:3 cr");
	cr_op(fs);
	cr_op(fs);
	cr_op(fs);

	printf("cr 4");
	cr_op(fs);
	cr_op(fs);
	cr_op(fs);
	cr_op(fs);

	printf("cr end\n");
	forth_release_state(fs);
	PASS();

	return 0;
}

int print_string() {
	struct forth_state* fs = forth_make_default_state();
	struct forth_byte_code* bc1 = forth_compile(".\"Hello Forth\" cr cr");
	struct forth_byte_code* bc2 = forth_compile(".\"Hello Forth\nmultiline\nline1\nline2\" cr");

	forth_run(fs, bc1);
	forth_run(fs, bc2);

	forth_release_state(fs);
	forth_release_byte_code(bc1);
	forth_release_byte_code(bc2);
	PASS();

	return 0;
}

int main(int argc, char** args) {
	push_pop();
	drop();
	swap();
	over();
	rot();

	dot();
	emit();
	cr();
	print_string();
	return 0;
}
