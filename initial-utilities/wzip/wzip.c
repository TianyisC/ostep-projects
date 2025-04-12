#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SWAP(a, b, type) do { type temp = (a); (a) = (b); (b) = temp; } while (0)

// space contains '\0'
void compressLine(char* line, size_t space)
{
  int pos = 0, cnt = 1;
  char ch = 0;
  while (pos < space-1) {
    if (cnt == 1)
       ch = line[pos];
    if (line[pos] == line[pos+1]) {
      cnt++;
    } else {
      fwrite(&cnt, sizeof(int), 1, stdout);
      fwrite(&ch, 1, 1, stdout);
      cnt = 1;
    }
    pos++;
  }
}

// enhanced strcat
void extend(char** dst, char* str)
{
  if (*dst == NULL) {
    *dst = malloc(strlen(str) + 1);
    strcpy(*dst, str);
  } else {
    char* temp = malloc(strlen(*dst) + strlen(str) + 1);
    strcpy(temp, *dst);
    strcat(temp, str);
    free(*dst);
    *dst = temp;
  }
}

int repeat(char* str, char ch) {
  while (*str != 0) {
    if (ch != *str++) 
      return 0;
  }
  return 1;
}

int main(int argc, char** argv)
{
  if (argc < 2) {
    printf("wzip: file1 [file2 ...]\n");
    return 1;
  }

    int firstRead = 1;
    char firstFileLastChar = 0, *combined = NULL, *crossfileCombo = NULL;
  for (int i = 1; i < argc; i++) 
  {
    FILE* fp = fopen(argv[i], "r");
    if (fp == NULL) {
      continue;
    }
    
    char* line = NULL;
    size_t n = 0;
    ssize_t nch = getline(&line, &n, fp);
    if (nch == -1) {
      free(line);
      fclose(fp);
      continue;  // go through next file
    }
    
    while (1) {
      extend(&combined, line);

      //TODO Fix the corner case, the continous file
      if (firstFileLastChar && firstRead && line[0] == firstFileLastChar) {
        while ( (nch = getline(&line, &n, fp)) != -1 && repeat(line, firstFileLastChar) ) {
          extend(&crossfileCombo, line);
        }
        
        if (nch == -1) {
          firstRead = 1;
          break;
        } else {
          firstRead = 0;
          compressLine(crossfileCombo, strlen(crossfileCombo) + 1);
          free(crossfileCombo);
          crossfileCombo = NULL;
          continue;
        }
      }

      if (line[nch-1] == '\n') {
        while ( (nch = getline(&line, &n, fp)) != -1 && line[0] == '\n') {
          extend(&combined, line);
        }
        compressLine(combined, strlen(combined) + 1);
        free(combined);
        combined = NULL;
        if (nch == -1) 
          break;
      } else { // the last line
        if (i == 1 && argc > 2) {
          firstFileLastChar = line[nch-1];
          extend(&crossfileCombo, line);
        } else {
          compressLine(combined, nch + 1);
        } 
        free(combined);
        combined = NULL;
        break;
      }
    }

    free(line);
    fclose(fp);
  }

  return 0;
}