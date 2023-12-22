#include "s21_string.h"

s21_size_t s21_strspn(const char *str, const char *sym) {
  s21_size_t result = 0;
  char *pointer1 = (char *)str;
  char *pointer2 = (char *)sym;
  for (; *pointer1 != '\0' && s21_strchr(pointer2, *pointer1++);) {
    result++;
  }
  return result;
}

/* Разбивает строку str на ряд токенов, разделенных delim. */

char *s21_strtok(char *str, const char *delim) {
  static char *pointer = S21_NULL;
  int string_end = 0;
  if (str != S21_NULL) {
    pointer = str;
  }
  if (pointer) {
    str = pointer + s21_strspn(pointer, delim);
    pointer = str + s21_strcspn(str, delim);
    if (pointer == str) {
      pointer = S21_NULL;
      string_end = 1;
    } else if (*pointer) {
      *pointer = 0;
      pointer = pointer + 1;
    } else {
      pointer = 0;
    }
  }
  return string_end == 0 ? str : S21_NULL;
}
