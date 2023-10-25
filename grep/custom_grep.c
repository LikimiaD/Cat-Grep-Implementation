#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define mods_array_list \
  { 'e', 'i', 'v', 'c', 'l', 'n', 'h', 's', 'f', 'o' }

typedef struct Mode {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} Mode;

void fill_struct(struct Mode* ptr);
int input_flags(struct Mode* ptr, char buffer[]);
int switch_flags(struct Mode* ptr, char sym);
int print_file(struct Mode* mods, char filename[], char** pattern_arr,
               int arr_len, int many_files_status);

int find_word(char buffer[], char word[], int mod_i);
int* find_word_handle_o(char buffer[], char word[], int mod_i, int* size);
void read_patterns_from_file(char* filename, char*** pattern_pointer,
                             int* pattern_index, int* patter_pointers_count);
char custom_to_lower(char sym);

int main(int avgc, char* avgv[]) {
  int start = 0;
  int status = 0;
  int many_files_status = 0;
  int patter_pointers_count = 10;
  Mode mods;
  fill_struct(&mods);
  if (avgc > 2) {
    int pattern_index = 0;
    char** pattern_pointer =
        (char**)malloc(sizeof(char*) * patter_pointers_count);
    for (int i = 1; i < avgc; i++) {
      if (input_flags(&mods, avgv[i]) != 0) {
        if (mods.f) {  // flag -f
          read_patterns_from_file(avgv[++i], &pattern_pointer, &pattern_index,
                                  &patter_pointers_count);
          start += 2;
        } else {
          start += 2;
          int pattern_len = strlen(avgv[++i]);
          if (pattern_index == patter_pointers_count) {
            pattern_pointer = (char**)realloc(
                pattern_pointer, sizeof(char*) * ++patter_pointers_count);
          }
          pattern_pointer[pattern_index] =
              (char*)malloc(sizeof(char) * (pattern_len + 1));
          strcpy(pattern_pointer[pattern_index], avgv[i]);
          strcat(pattern_pointer[pattern_index], "\0");
          pattern_index++;
        }
      } else {
        break;
      }
    }
    if (start != (avgc - 1)) {
      if ((avgc - 1) != (start + 1)) {
        many_files_status = 1;
      }
      for (int i = start + 1; i < avgc; i++) {
        status = print_file(&mods, avgv[i], pattern_pointer, pattern_index,
                            many_files_status);
      }
    } else {
      printf(
          "grep: No information about file. Example: grep [option...] "
          "[patterns] [file...]");
      status = 1;
    }
    for (int i = 0; i < pattern_index; i++) {
      free(pattern_pointer[i]);
    }
    free(pattern_pointer);
  } else {
    printf("grep: Bad call. Example: grep [option...] [patterns] [file...]");
    status = 1;
  }
  return status;
}

void fill_struct(struct Mode* ptr) {
  ptr->l = 0;
  ptr->c = 0;
  ptr->e = 0;
  ptr->f = 0;
  ptr->h = 0;
  ptr->i = 0;
  ptr->n = 0;
  ptr->o = 0;
  ptr->s = 0;
  ptr->v = 0;
}

int input_flags(struct Mode* ptr, char buffer[]) {
  int flags_detected = 0;

  if (buffer[0] == '-') {
    for (int i = 1; buffer[i] != '\0'; i++) {
      if (switch_flags(ptr, buffer[i])) {
        flags_detected++;
      } else {
        return 0;
      }
    }
  }
  return flags_detected;
}

int switch_flags(struct Mode* ptr, char sym) {
  int status = 1;
  switch (sym) {
    case 'l':
      ptr->l = 1;
      break;
    case 'c':
      ptr->c = 1;
      break;
    case 'e':
      ptr->e = 1;
      break;
    case 'f':
      ptr->f = 1;
      break;
    case 'h':
      ptr->h = 1;
      break;
    case 'i':
      ptr->i = 1;
      break;
    case 'n':
      ptr->n = 1;
      break;
    case 'o':
      ptr->o = 1;
      break;
    case 's':
      ptr->s = 1;
      break;
    case 'v':
      ptr->v = 1;
      break;
    default:
      status = 0;
      break;
  }
  return status;
}

void read_patterns_from_file(char* filename, char*** pattern_pointer,
                             int* pattern_index, int* patter_pointers_count) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    perror("Failed to open pattern file");
    return;
  }

  char* line = NULL;
  size_t len = 0;
  while (getline(&line, &len, file) != -1) {
    // Remove the newline character
    line[strcspn(line, "\n")] = 0;

    if (*pattern_index == *patter_pointers_count) {
      *pattern_pointer = (char**)realloc(
          *pattern_pointer, sizeof(char*) * ++(*patter_pointers_count));
    }

    (*pattern_pointer)[*pattern_index] = strdup(line);
    (*pattern_index)++;
  }
  fclose(file);
  free(line);
}

int find_word(char buffer[], char word[], int mod_i) {
  int status = 0;
  int buffer_len = strlen(buffer);
  int word_len = strlen(word);
  char buffer_sym = 0;
  char word_sym = 0;
  for (int i = 0; i < buffer_len; i++) {
    buffer_sym = buffer[i];
    word_sym = word[0];
    if (mod_i == 1) {
      buffer_sym = custom_to_lower(buffer_sym);
      word_sym = custom_to_lower(word_sym);
    }
    if (buffer_sym == word_sym) {
      status = 1;
      for (int j = 1; j < word_len; j++) {
        buffer_sym = buffer[++i];
        word_sym = word[j];
        if (mod_i == 1) {
          buffer_sym = custom_to_lower(buffer_sym);
          word_sym = custom_to_lower(word_sym);
        }
        if (buffer_sym != word_sym) {
          status = 0;
          break;
        }
      }
    }
    if (status == 1) {
      break;
    }
  }
  return status;
}

int* find_word_handle_o(char buffer[], char word[], int mod_i, int* size) {
  int buffer_len = strlen(buffer);
  int word_len = strlen(word);
  char buffer_sym = 0;
  char word_sym = 0;

  int* start_indices = malloc(buffer_len * sizeof(int));

  *size = 0;

  for (int i = 0; i < buffer_len; i++) {
    buffer_sym = buffer[i];
    word_sym = word[0];
    if (mod_i == 1) {
      buffer_sym = custom_to_lower(buffer_sym);
      word_sym = custom_to_lower(word_sym);
    }

    if (buffer_sym == word_sym) {
      int status = 1;
      int k = i;
      for (int j = 1; j < word_len; j++) {
        buffer_sym = buffer[++k];
        word_sym = word[j];
        if (mod_i == 1) {
          buffer_sym = custom_to_lower(buffer_sym);
          word_sym = custom_to_lower(word_sym);
        }
        if (buffer_sym != word_sym) {
          status = 0;
          break;
        }
      }

      if (status == 1) {
        start_indices[(*size)++] = i;
      }
    }
  }

  return start_indices;
}

char custom_to_lower(char sym) {
  int ascii = (int)sym;
  if ((ascii >= 65) && (ascii <= 90)) {
    ascii += 32;
  }
  return (char)ascii;
}

int print_file(struct Mode* mods, char filename[], char** pattern_arr,
               int arr_len, int many_files_status) {
  int status = 0;
  FILE* ptr = fopen(filename, "r");
  if (ptr == NULL && mods->s != 1) {  // Flag -s
    printf("grep: %s: No such file or directory\n", filename);
    return 1;
  }
  char* line = 0;  // Dynamic line from FILE
  size_t len = 0;  // Buffer size
  int total_count = 0;
  int line_num = 0;
  int size = 0;
  int* start_indices = NULL;
  while (getline(&line, &len, ptr) != -1) {
    ++line_num;
    int line_contains_pattern = 0;
    for (int i = 0; i < arr_len; i++) {
      if (mods->o != 1) {
        if (find_word(line, pattern_arr[i], mods->i) == 1) {
          line_contains_pattern = 1;  // Find word in line
          break;
        }
      } else if (mods->o == 1) {
        if (mods->l == 1) {
          printf("%s\n", filename);
        }
        start_indices =
            find_word_handle_o(line, pattern_arr[i], mods->i, &size);
        if (size > 0) {
          line_contains_pattern = 1;
          for (int j = 0; j < size; j++) {
            if (many_files_status == 1 && mods->h != 1) {
              printf("%s:", filename);
            }
            if (mods->n == 1) {
              printf("%d:", line_num);
            }
            printf("%.*s\n", (int)strlen(pattern_arr[i]),
                   line + start_indices[j]);
          }
        }
        free(start_indices);
        continue;
      }
    }
    if (mods->o == 1) {
      continue;
    }
    // Apply -v flag
    if (mods->v == 1) {
      line_contains_pattern = !line_contains_pattern;
    }
    if (line_contains_pattern != 0 || start_indices != NULL) {
      total_count++;
      if (mods->l == 1 && mods->h != 1) {  // Flag -l
        printf("%s\n", filename);
        break;
      }
      if (many_files_status == 1 && mods->c != 1 && mods->h != 1) {
        printf("%s:", filename);
      }
      if (mods->n == 1 && mods->c != 1) {
        printf("%d:", line_num);
      }
      if (mods->c != 1) {
        printf("%s", line);
        if (line[strlen(line) - 1] != '\n') {
          printf("\n");
        }
      }
    }
  }
  if (mods->c == 1) {
    if (mods->h != 1 && many_files_status != 0) {
      printf("%s:%d\n", filename, total_count);
    } else {
      printf("%d\n", total_count);
    }
  }
  free(line);  // Clean Dynamic line
  fclose(ptr);
  return status;
}