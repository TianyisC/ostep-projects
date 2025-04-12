#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv)
{
  if (argc < 2) {
    printf("wunzip: file1 [file2 ...]\n");
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    FILE* fp = fopen(argv[i], "rb");
    if (fp == NULL) {
      continue;
    }

    while (!feof(fp) && !ferror(fp)) {
      int num = 0;
      char ch = 0;
      int rc = fread(&num, sizeof(int), 1, fp);
      rc |= fread(&ch, 1, 1, fp);

      for (int i = 0; rc && i < num; i++) {
        printf("%c", ch);
      }
    }
    fflush(stdin);
    fclose(fp);
  }

  return 0;
}