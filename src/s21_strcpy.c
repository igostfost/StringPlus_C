#include "s21_string.h"

/*
Функция s21_strcpy копирует содержимое строки src в строку dest.
Если dest и src не равны S21_NULL.
В конце возвращается указатель на начало строки dest. */

char *s21_strcpy(char *dest, const char *src) {
  s21_size_t i;
  s21_size_t len = s21_strlen(src);
  for (i = 0; i < len; i++) {
    dest[i] = src[i];
  }
  dest[i] = '\0';
  return dest;
}
