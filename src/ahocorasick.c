#include "ahocorasick.h"

#include <string.h>
#include <stdint.h>

#define STATE_GET(ahocorasick, state)		((ahocorasick)->state_array[(state)])

bool ahocorasick_add_state(ahocorasick_t *ahocorasick) {
	if(!ahocorasick) {
		return false;
	}
	
	if(!ahocorasick->state_array) {
		ahocorasick->state_array_count = 0;
		ahocorasick->state_array_size = 8;
		ahocorasick->state_array = calloc(ahocorasick->state_array_size, sizeof(*ahocorasick->state_array));
	} else if(ahocorasick->state_array_count == ahocorasick->state_array_size) {
		ahocorasick->state_array_size *= 2;
		ahocorasick->state_array = realloc(ahocorasick->state_array, ahocorasick->state_array_size*sizeof(*ahocorasick->state_array));
		memset(ahocorasick->state_array + ahocorasick->state_array_count, 0, ahocorasick->state_array_count * sizeof(*ahocorasick->state_array));
	}
	
	ahocorasick->state_array_count++;
	
	return true;
}

bool ahocorasick_state_add_output(ahocorasick_t *ahocorasick, size_t state, size_t output) {
	if(!ahocorasick) {
		return false;
	}
	
	if(!STATE_GET(ahocorasick, state).output_array) {
		STATE_GET(ahocorasick, state).output_array_count = 0;
		STATE_GET(ahocorasick, state).output_array_size = 2;
		STATE_GET(ahocorasick, state).output_array = calloc(STATE_GET(ahocorasick, state).output_array_size, sizeof(*STATE_GET(ahocorasick, state).output_array));
	} else if(STATE_GET(ahocorasick, state).output_array_count == STATE_GET(ahocorasick, state).output_array_size) {
		STATE_GET(ahocorasick, state).output_array_size *= 2;
		STATE_GET(ahocorasick, state).output_array = realloc(STATE_GET(ahocorasick, state).output_array, STATE_GET(ahocorasick, state).output_array_size*sizeof(*STATE_GET(ahocorasick, state).output_array));
		memset(STATE_GET(ahocorasick, state).output_array + STATE_GET(ahocorasick, state).output_array_count, 0, STATE_GET(ahocorasick, state).output_array_count * sizeof(*STATE_GET(ahocorasick, state).output_array));
	}
	
	STATE_GET(ahocorasick, state).output_array[STATE_GET(ahocorasick, state).output_array_count] = output;
	STATE_GET(ahocorasick, state).output_array_count++;
	
	return true;
}

bool ahocorasick_state_add_keyword(ahocorasick_t *ahocorasick, size_t state, char *str, size_t len, size_t output) {
	if(!ahocorasick) {
		return false;
	}
	
	if(len <= 0) {
		return ahocorasick_state_add_output(ahocorasick, state, output);
	}
	
	if(!STATE_GET(ahocorasick, state).childs[(uint8_t)str[0]]) {
		if(!ahocorasick_add_state(ahocorasick)) {
			return false;
		}
		STATE_GET(ahocorasick, state).childs[(uint8_t)str[0]] = ahocorasick->state_array_count - 1;
	}
	
	return ahocorasick_state_add_keyword(ahocorasick, STATE_GET(ahocorasick, state).childs[(uint8_t)str[0]], str + 1, len - 1, output);
}

bool ahocorasick_add_keyword(ahocorasick_t *ahocorasick, char *str, size_t len, size_t output) {
	if(!ahocorasick || !str) {
		return false;
	}
	
	if(!ahocorasick->state_array && !ahocorasick_add_state(ahocorasick)) {
		return false;
	}
	
	return ahocorasick_state_add_keyword(ahocorasick, 0, str, len, output);
}

bool ahocorasick_update_fail(ahocorasick_t *ahocorasick) {
	if(!ahocorasick) {
		return false;
	}
	
	size_t i, k, *queue = NULL, read = 0, write = 0, state = 0, r = 0, s = 0;
	queue = calloc(ahocorasick->state_array_count, sizeof(ahocorasick->state_array[0].childs[0]));
	
	for(i = 0; i < 256; i++) {
		if(ahocorasick->state_array[0].childs[i]) {
			queue[write] = ahocorasick->state_array[0].childs[i];
			write++;
		}
	}
	
	while(write > read) {
		r = queue[read];
		read++;
		
		for(i = 0; i < 256; i++) {
			s = STATE_GET(ahocorasick, r).childs[i];
			if(s) {
				queue[write] = s;
				write++;
				
				state = STATE_GET(ahocorasick, r).fail;
				while(state && !STATE_GET(ahocorasick, state).childs[i] && STATE_GET(ahocorasick, state).fail) {
					state = STATE_GET(ahocorasick, state).fail;
				}
				STATE_GET(ahocorasick, s).fail = STATE_GET(ahocorasick, state).childs[i];
				for(k = 0; k < STATE_GET(ahocorasick, STATE_GET(ahocorasick, s).fail).output_array_count; k++) {
					ahocorasick_state_add_output(ahocorasick, s, STATE_GET(ahocorasick, STATE_GET(ahocorasick, s).fail).output_array[k]);
				}
			}
		}
	}
	
	free(queue);
	
	return true;
}

bool ahocorasick_finalize(ahocorasick_t *ahocorasick) {
	if(!ahocorasick) {
		return false;
	}
	
	ahocorasick_update_fail(ahocorasick);

/*	
	size_t i;
	for(i = 1; i < ahocorasick->state_array_count; i++) {
		printf("i: %lu, f(i): %lu\n", i, ahocorasick->state_array[i].fail);
	}
*/

	return true;
}

void ahocorasick_clean(ahocorasick_t *ahocorasick) {
	size_t i;
	for(i = 0; i < ahocorasick->state_array_count; i++) {
		if(STATE_GET(ahocorasick, i).output_array) {
			free(STATE_GET(ahocorasick, i).output_array);
			STATE_GET(ahocorasick, i).output_array_count = 0;
			STATE_GET(ahocorasick, i).output_array_size = 0;
		}
	}
	free(ahocorasick->state_array);
	ahocorasick->state_array_count = 0;
	ahocorasick->state_array_size = 0;
}

bool ahocorasick_to_dot(ahocorasick_t *ahocorasick, FILE *f) {
	if(!ahocorasick || !f) {
		return false;
	}
	
	fprintf(f, "digraph G {\n");
	
	size_t i, j;
	for(i = 0; i < ahocorasick->state_array_count; i++) {
		if(ahocorasick->state_array[i].output_array_count > 0) {
			fprintf(f, "\t%lu [shape=doublecircle]\n", i);
		}
		for(j = 0; j < 256; j++) {
			if(ahocorasick->state_array[i].childs[j]) {
				fprintf(f, "\t%lu -> %lu [label=\"%c\"];\n", i, ahocorasick->state_array[i].childs[j], (char)j);
			}
		}
	}
	
	fprintf(f, "}\n");
	
	return true;
}