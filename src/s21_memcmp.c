#include "s21_string.h"

/* Сравнивает первые n байтов str1 и str2. */

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int res = 0;
  unsigned char *ptr1 = (unsigned char *)str1;
  unsigned char *ptr2 = (unsigned char *)str2;
  for (s21_size_t i = 0; i < n; i++) {
    if (*ptr1 != *ptr2) {
      res = *ptr1 - *ptr2;
      break;
    }
    ptr1++;
    ptr2++;
  }
  return res;
}
