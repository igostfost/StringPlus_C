#include "s21_string.h"

/* Копирует n символов из src в dest. */

void *s21_memcpy(void *dst, const void *src, s21_size_t n) {
  unsigned char *ptr_dst = (unsigned char *)dst;
  unsigned char *ptr_src = (unsigned char *)src;

  if ((dst != S21_NULL) && (src != S21_NULL)) {
    for (s21_size_t i = 0; i < n; i++) {
      ptr_dst[i] = ptr_src[i];
    }
  }

  return ptr_dst;
}
