#include "forth_embed.h"

const char* program1 = ": is-it-zero? 0 = if .\" Да!\" else .\" Нет!\" then  0 . cr ; 0 is-it-zero? 1 is-it-zero? 2 is-it-zero?";
const char* program2 = "Да!\"";
const char* program3 = ": loop-test 10 0 do 5 . cr loop 30 . ; loop-test";
const char* program4 = ": print-keycode -1 begin 1 + dup 3 = invert dup .  until ; print-keycode";
const char* program5 = "42 constant answer 2 answer * .";
const char* program6 = "variable hello 3 cells allot 1 hello 0 cells + ! 2 hello 1 cells + ! 3 hello 2 cells + ! hello 0 cells + @ .";
const char* program7 = ": loop-test 1 2 3 4 5 6 3 0 do 2 0 i . cr do i . cr loop loop ; loop-test";
const char* program8 = ": fib-iter 0 1 rot 0 do over + swap loop drop ; 10 fib-iter .";
const char* program9 = "10 0 1 rot . . .";
const char* program10 = ": aa .\" asdasdasd\" ; : vv .\" rrrrrrrrrrrrrrr\" aa aa aa aa ; aa vv";
const char* program11 = ": aa asdf ; aa . . . ";

void asdf(struct forth_state* fs) {
	forth_data_stack_push(fs, 1);
	forth_data_stack_push(fs, 2);
	forth_data_stack_push(fs, 3);
}

int main(int argc, char** args) {
	struct forth_byte_code* program = forth_compile(program8);
	struct forth_state* fs = forth_make_default_state();

	//forth_set_function(fs, "asdf", asdf);
	forth_run(fs, program);
	//forth_run_function(fs, program, "aa");
	forth_release_state(fs);
	forth_release_byte_code(program);
	return 0;
}
