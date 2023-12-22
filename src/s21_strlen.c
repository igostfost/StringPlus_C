#include "s21_string.h"
/**
 * Вычисляет длину строки str, не включая завершающий нулевой символ.
 * str - указатель на строку.
 * return - количество символов в строке до первого вхождения символа конца
 * строки.
 */

s21_size_t s21_strlen(const char *str) {
  s21_size_t count = 0;
  const char *ptr = str;
  while (*ptr != '\0') {
    ptr++;
    count++;
  }
  return count;
}
