#include "s21_string.h"

/* Копирует символ c (беззнаковый тип)
в первые n символов строки, на которую указывает аргумент str. */

void *s21_memset(void *dst, int c, s21_size_t n) {
  unsigned char *ptr_dst = (unsigned char *)dst;
  for (s21_size_t i = 0; i < n; i++) {
    ptr_dst[i] = c;
  }

  return ptr_dst;
}
