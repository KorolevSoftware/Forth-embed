#include "forth_embed.h"
#include <stdio.h>
#include <assert.h>

#define PASS() printf("Pass %s\n", __func__);

static const char* fizzbuzz = ""
": fizz? 3 mod 0 = dup if .\" Fizz\" then ; "
": buzz? 5 mod 0 = dup if .\" Buzz\" then ; "
": fizz-buzz? dup fizz? swap buzz? or invert ; "
": do-fizz-buzz  25 1 do cr i fizz-buzz? if i . then loop ; "
"do-fizz-buzz cr";

static const char* test_loop = ": loop-test 10 0 do i . loop ; loop-test cr";
static const char* fibiter = ": fib-iter 0 1 rot 0 do over + swap loop drop ; 3 fib-iter . cr 30 fib-iter .";

int code_tester(const char* code) {
	struct forth_state* fs = forth_make_default_state();
	struct forth_byte_code* bc = forth_compile(code);

	forth_run(fs, bc);

	forth_release_state(fs);
	forth_release_byte_code(bc);
	PASS();

	return 0;
}

int main(int argc, char** args) {
	code_tester(fizzbuzz);
	code_tester(test_loop);
	code_tester(fibiter);
	return 0;
}
