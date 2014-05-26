#ifndef _AHOCORASICK_H_
#define _AHOCORASICK_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct ahocorasick_state_s {
	size_t childs[256];
	size_t fail;
	size_t *output_array;
	size_t output_array_count;
	size_t output_array_size;
} ahocorasick_state_t;

typedef struct ahocorasick_s {
	ahocorasick_state_t *state_array;
	size_t state_array_count;
	size_t state_array_size;
} ahocorasick_t;

bool ahocorasick_init(ahocorasick_t *ahocorasick);
bool ahocorasick_add_keyword(ahocorasick_t *ahocorasick, char *str, size_t len, size_t output);
bool ahocorasick_finalize(ahocorasick_t *ahocorasick);
bool ahocorasick_clean(ahocorasick_t *ahocorasick);

bool ahocorasick_to_dot(ahocorasick_t *ahocorasick, FILE *f);

#endif