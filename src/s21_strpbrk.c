#include "s21_string.h"

/* Находит первый символ в строке str1,
который соответствует любому символу, указанному в str2. */

char *s21_strpbrk(const char *str1, const char *str2) {
  char *match = S21_NULL;
  int flag = 0;
  for (int i = 0; str1[i] != '\0' && !flag; i++) {
    for (int j = 0; str2[j] != '\0' && !flag; j++) {
      if (str2[j] == str1[i]) {
        match = (char *)&str1[i];
        flag = 1;
        break;
      }
    }
  }
  return match;
}
