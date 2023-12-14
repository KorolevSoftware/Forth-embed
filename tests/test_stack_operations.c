#include "forth_embed.h"
#include <stdio.h>
#include <assert.h>

#define PASS() printf("Pass %s\n", __func__);


// testing forth componets
void drop_op(struct forth_state* fs);
void swap_op(struct forth_state* fs);
void over_op(struct forth_state* fs);
void rot_op(struct forth_state* fs);


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

int main(int argc, char** args) {
	push_pop();
	drop();
	swap();
	over();
	rot();
	return 0;
}
