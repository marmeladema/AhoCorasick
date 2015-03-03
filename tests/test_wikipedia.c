#include "ahocorasick.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	ahocorasick_t ahocorasick;
	if(!ahocorasick_init(&ahocorasick)) {
		fprintf(stderr, "could not initialize ahocorasick\n");
		return 1;
	}

	char *keywords[] = {
		"a", "ab", "bab", "bc", "bca", "c", "caa"
	};

	size_t i;
	for(i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
		if(!ahocorasick_add_keyword(&ahocorasick, keywords[i], strlen(keywords[i]), i)) {
			fprintf(stderr, "could not add \"%s\" to ahocorasick\n", keywords[i]);
			return 1;
		}
	}

	ahocorasick_finalize(&ahocorasick);

	if(argc >= 2 && strcmp(argv[1], "--dot") == 0) {
		ahocorasick_to_dot(&ahocorasick, stdout);
	} else if(argc >= 2 && strcmp(argv[1], "--size") == 0) {
		printf("AhoCorasick size: %lu bytes\n", ahocorasick_size(&ahocorasick));
	} else {
		ahocorasick_match_t match;
		ahocorasick_match_init(&match, "abccab", 6);

		size_t output;
		while(ahocorasick_match(&ahocorasick, &match, &output)) {
			printf("Match found \"%s\" at position %lu\n", keywords[output], 6-match.len);
		}
	}

	ahocorasick_clean(&ahocorasick);

	return 0;
}
