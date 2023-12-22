#include "s21_string.h"

/* Выполняет поиск последнего вхождения символа
 c (беззнаковый тип) в строке, на которую указывает аргумент str. */

char *s21_strrchr(const char *str, int c) {
  const char *pointer = S21_NULL;
  const char *pointer1 = str;  // указатель на константную переменную str
  for (; *pointer1 != '\0'; pointer1++) {
    if (*pointer1 == (char)c) {
      pointer = pointer1;
    }
  }
  if (*pointer1 == '\0' && c == 0) {
    pointer = pointer1;
  }
  return (char *)pointer;
}
