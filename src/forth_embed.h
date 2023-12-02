#pragma once

// Public API

// Interface struct
struct forth_byte_code;
struct forth_state;

// forth stack manipulation
void forth_data_stack_push(struct forth_state* fs, int value);
int forth_data_stack_pop(struct forth_state* fs);


// Create forth stack from run/eval program
struct forth_state* make_default_state();
void release_state(struct forth_state* fs);

typedef void (*forth_native_function)(struct forth_state* fs);

// Set user constants, variables or functions
void forth_set_constant(struct forth_state* fs, const char* name, int value);
void forth_set_function(struct forth_state* fs, const char* name, forth_native_function func);


// Run code or function
void forth_run(struct forth_state* fs, const struct forth_byte_code* script);


// Compile and release functions
const struct forth_byte_code* forth_compile(const char* script);
void release_forth_byte_code(struct forth_byte_code* fbc);