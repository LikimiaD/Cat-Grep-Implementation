#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define mods_array_list \
  { 'b', 'e', 'E', 'n', 's', 't', 'T', 'v' }
#define tab_ascii_nym 9
#define new_line_ascii_num 10

typedef struct Mode {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} Mode;

void struct_fill(struct Mode* mods);
void struct_change_value(struct Mode* mods, char sym);
int print_file(char filename[], struct Mode mods);
int check_mode(struct Mode* mods, char buffer[]);
int handle_space_lines(struct Mode mods, char* line, int* space);
char* handle_tabs(struct Mode mods, char* line, int* line_len);
void handle_line_numbers(struct Mode mods, char* line, int* line_count);

int main(int argc, char* argv[]) {
  // First value we need skip -> it's ./a.out
  Mode mods;
  struct_fill(&mods);
  int status = 0;
  if (argc > 2) {
    int start_value = 0;
    for (int i = 1; i < argc; i++) {
      if (check_mode(&mods, argv[i]) != -1) {
        start_value++;
      } else {
        start_value++;
        break;
      }
    }
    for (int i = start_value; i < argc; i++) {
      status = print_file(argv[i], mods);
    }
  } else if (argc > 1) {
    print_file(argv[1], mods);
  } else {
    printf("cat: invalid file, no iformation about file.");
    status = 1;
  }
  return status;
}

void struct_fill(struct Mode* mods) {
  mods->b = 0;
  mods->e = 0;
  mods->n = 0;
  mods->s = 0;
  mods->t = 0;
  mods->v = 0;
}

void struct_change_value(struct Mode* mods, char sym) {
  switch (sym) {
    case 'b':
      mods->b = 1;
      break;
    case 'E':
      mods->e = 1;
      break;
    case 'e':
      mods->e = 1;
      mods->v = 1;
      break;
    case 'n':
      mods->n = 1;
      break;
    case 's':
      mods->s = 1;
      break;
    case 'T':
      mods->t = 1;
      break;
    case 't':
      mods->t = 1;
      mods->v = 1;
      break;
    case 'v':
      mods->v = 1;
      break;
    default:
      break;
  }
}

int check_mode(struct Mode* mods, char buffer[]) {
  int status = -1;  // -1 -> doesn't have mode; >0 -> have
  if ((int)buffer[0] == (int)'-') {
    int len = strlen(buffer);
    if (len > 1 && (int)buffer[1] == (int)'-') {
      if (strcmp(buffer, "--number-nonblank") == 0) {
        mods->b = 1;
        status = 0;
      } else if (strcmp(buffer, "--number") == 0) {
        mods->n = 1;
        status = 2;
      } else if (strcmp(buffer, "--squeeze-blank") == 0) {
        mods->s = 1;
        status = 3;
      }
    } else {
      char mods_array[] = mods_array_list;
      size_t size = sizeof(mods_array) / sizeof(char);
      for (int i = 0; i < len; i++) {
        for (size_t j = 0; j < size; j++) {
          if ((int)buffer[i] == (int)mods_array[j]) {
            struct_change_value(mods, mods_array[j]);
            status = i;
            break;
          }
        }
      }
    }
  }
  return status;
}

int handle_space_lines(struct Mode mods, char* line, int* space) {
  int status = 0;
  if (mods.s == 1 && line[0] == '\n') {  // Check space-lines
    if (*space != 0) {
      status = 1;
    } else {
      *space = 1;
    }
  } else {
    *space = 0;
  }
  return status;
}

char* handle_tabs(struct Mode mods, char* line, int* line_len) {
  if (mods.t == 1) {
    int tabs_count = 0;
    for (int i = 0; i < *line_len; i++) {
      if (((int)(line[i] == tab_ascii_nym))) {
        tabs_count++;
      }
    }
    if (tabs_count != 0) {
      char* new_line = malloc(*line_len + 2 * tabs_count + 1);
      int new_line_index = 0;
      for (int i = 0; i < *line_len; i++) {
        if (line[i] == '\t') {
          new_line[new_line_index++] = '^';
          new_line[new_line_index++] = 'I';
        } else {
          new_line[new_line_index++] = line[i];
        }
      }
      new_line[new_line_index] = '\0';
      free(line);
      line = new_line;
      *line_len = new_line_index;
    }
  }
  return line;
}

void handle_line_numbers(struct Mode mods, char* line, int* line_count) {
  if (mods.n == 1 && mods.b != 1) {  // All num
    printf("%6d\t", *line_count);
    (*line_count)++;
  } else if (mods.b == 1 && line[0] != '\n') {  // Num but dodge space-lines
    printf("%6d\t", *line_count);
    (*line_count)++;
  }
}

int print_file(char filename[], struct Mode mods) {
  int status = 0;
  FILE* ptr = fopen(filename, "r");
  if (ptr == NULL) {
    printf("cat: %s: No such file or directory\n", filename);
    status = 1;
    return status;
  } else {
    int line_count = 1;
    int space = 0;  // 0 -> before not space-line, 1 -> before was space-line
    int line_len = 0;
    char* line = 0;  // Dynamic line from FILE
    size_t len = 0;  // Buffer size
    while (getline(&line, &len, ptr) != -1) {
      line_len = strlen(line);
      if (handle_space_lines(mods, line, &space) == 1) {
        continue;
      };
      line = handle_tabs(mods, line, &line_len);
      handle_line_numbers(mods, line, &line_count);

      if (mods.v == 1 || mods.e == 1 || mods.t == 1) {
        for (int i = 0; i < line_len; i++) {
          if ((unsigned char)line[i] < 32 && line[i] != '\n' &&
              line[i] != '\t' && line[i] != '\0' && mods.v == 1) {
            printf("^%c", line[i] + '@');
          } else {
            if (line[i] == '\n' && mods.e == 1) {
              printf("$%c", line[i]);
            } else {
              printf("%c", line[i]);
            }
          }
        }
      } else {
        printf("%s", line);
      }
    }
    free(line);  // Clean Dynamic line
    fclose(ptr);
  }
  return status;
}