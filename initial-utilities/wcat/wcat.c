#include <stdio.h>


int main(int argc, char** argv)
{
	if (argc < 2) 
		return 0;

	for (int i = 1; i < argc; i++) {
		FILE* fp = fopen(argv[i], "r");
		if (fp == NULL) {
			printf("wcat: cannot open file\n");
			return 1;
		}

		char buf[4096] = {0};
		while (fgets(buf, 4096, fp) != NULL) {
			printf("%s", buf);
		}

	}

	return 0;
}
