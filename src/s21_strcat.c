#include "s21_string.h"

/*
Функция strcat добавляет в строку, на которую указывает аргумент dest,
строку, на которую указывает аргумент str.
Символ конца строки помещается в конце объединенных строк.
*/

char *s21_strcat(char *dest, const char *src) {
  char *buffer;
  buffer = (char *)dest;
  while (*buffer != '\0') {
    buffer++;
  }
  while (*src != '\0') {
    *buffer++ = *src++;
  }
  *buffer = '\0';

  return dest;
}
