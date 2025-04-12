#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readlineAndMatch(FILE* fp, const char* match)
{
  char* line = NULL;
  size_t len = 0;

  while(getline(&line, &len, fp) != -1 ) {
    if (strstr(line, match)) {
      printf("%s", line);
    }
  }
  free(line);
}

int main(int argc, char** argv)
{
  if (argc < 2) {
    printf("wgrep: searchterm [file ...]\n");
    return 1;
  }

  if (argc == 2) {
    // read from stdin
    readlineAndMatch(stdin, argv[1]);
    return 0;
  }

  for (int i = 2; i < argc; i++) {

    FILE* fp = fopen(argv[i], "r");
    if (fp == NULL) {
      printf("wgrep: cannot open file\n");
      return 1;
    }
    readlineAndMatch(fp, argv[1]);
    fclose(fp);
  }
  return 0;
}