#include "ahocorasick.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
	ahocorasick_t ahocorasick;
	memset(&ahocorasick, 0, sizeof(ahocorasick));
	
	if(!ahocorasick_add_keyword(&ahocorasick, "he", 2, 0)) {
		fprintf(stderr, "could not add \"he\" to ahocorasick\n");
	}
	if(!ahocorasick_add_keyword(&ahocorasick, "she", 3, 0)) {
		fprintf(stderr, "could not add \"she\" to ahocorasick\n");
	}
	if(!ahocorasick_add_keyword(&ahocorasick, "his", 3, 0)) {
		fprintf(stderr, "could not add \"she\" to ahocorasick\n");
	}
	if(!ahocorasick_add_keyword(&ahocorasick, "hers", 4, 0)) {
		fprintf(stderr, "could not add \"she\" to ahocorasick\n");
	}
	
	ahocorasick_finalize(&ahocorasick);
	
	ahocorasick_to_dot(&ahocorasick, stdout);
	
	ahocorasick_clean(&ahocorasick);
	
	return 0;
}