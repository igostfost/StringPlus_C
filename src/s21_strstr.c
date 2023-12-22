#include "s21_string.h"

/* Функция s21_strstr осуществляет поиск подстроки needle в строке haystack.
Она ищет первое вхождение подстроки needle в строке haystack
и возвращает указатель на начало найденной подстроки в строке haystack.
Если подстрока не найдена, функция возвращает S21_NULL. */

int sravnen(const char *haystack, const char *needle) {
  int result = 0;
  int break_flag = 0;
  while (*haystack && *needle && !break_flag) {
    if (*haystack != *needle) {
      result = 0;
      break_flag = 1;
    }

    haystack++;
    needle++;
  }
  result = (*needle == '\0');
  return result;
}

char *s21_strstr(const char *haystack, const char *needle) {
  const char *result = S21_NULL;
  int flag = 1;
  int break_flag = 0;

  if (s21_strlen(haystack) == 0 && s21_strlen(needle) == 0) {
    flag = 0;
    result = "";
  }
  while (*haystack && !break_flag) {
    if ((*haystack == *needle && sravnen(haystack, needle)) ||
        (*haystack == '\0' || *needle == '\0')) {
      result = haystack;
      flag = 0;
      break_flag = 1;
    }
    haystack++;
  }

  return flag ? S21_NULL : (char *)result;
}
