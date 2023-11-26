#include "forth_embed.h"

const char* program1 = ": is-it-zero? 0 = if .\" Да!\" else .\" Нет!\" then ; 0 is-it-zero? 1 is-it-zero? 2 is-it-zero?";
const char* program2 = "Да!\"";
const char* program3 = ": loop-test 10 0 do 5 . loop 30 . ; loop-test";


int main(int argc, char** args) {
	struct forth_byte_code* program = compile(program3);
	run(program);
	return 0;
}
