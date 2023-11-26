#pragma once

struct forth_byte_code;

const struct forth_byte_code* compile(const char* script);
void run(struct forth_byte_code* script);



