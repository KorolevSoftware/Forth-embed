# Forth-embed

Data type only integer
## Coming soon release! check todo

VS-Code support!

Stack operations:
- dup
- drop
- swap
- over
- rot

Prints:
- emit
- cr
- ."

Print example: ```."hello world"```

Сomparisons:
- =
- <
- \>
- invert
- and
- or

Math:
- \+
- \-
- /
- \*
- mod

 Controll flow:
 - if
 - else
 - then
 - do
 - i
 - loop
 - begin
 - until
 - :
 - ;

Variables/Constants
- constant
- variable
- allot
- cells
- @
- !


```C
#include "forth_embed.h"

void asdf(struct forth_state* fs) {
	forth_data_stack_push(fs, 1);
	forth_data_stack_push(fs, 2);
	forth_data_stack_push(fs, 3);
} 

struct forth_state* fs = forth_make_default_state();
struct forth_byte_code* program = forth_compile(": aa asdf ; aa . . .");
forth_set_function(fs, "asdf", asdf);
forth_run(fs, program);
forth_run_function(fs, program, "aa");
forth_release_state(fs);
forth_release_byte_code(program); 
```

# TODO
- make stable release
- write more tests

# Usings

[Sima-Engine](https://github.com/KorolevSoftware/Sima-Engine) - game engine with forth from script engine
