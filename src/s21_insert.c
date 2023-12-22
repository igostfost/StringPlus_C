#include "s21_string.h"

/* Возвращает новую строку, в которой указанная строка (str) вставлена в
 указанную позицию (start_index) в данной строке (src). В случае какой-либо
 ошибки следует вернуть значение S21_NULL */

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (src == S21_NULL || str == S21_NULL) {
    return S21_NULL;  // Проверка на нулевые указатели
  }

  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);

  if (start_index > src_len) {
    return S21_NULL;  // Проверка на корректность позиции вставки
  }

  int new_str_len = src_len + str_len;
  char *new_str = malloc((new_str_len + 1) * sizeof(char));

  if (new_str == S21_NULL) {
    return S21_NULL;  // Проверка на успешное выделение памяти
  }

  s21_memcpy(new_str, src, start_index);  // Копирование начала строки src

  s21_memcpy(new_str + start_index, str, str_len);  // Вставка строки str

  s21_memcpy(new_str + start_index + str_len, src + start_index,
             src_len - start_index);  // Копирование оставшейся части строки src

  new_str[new_str_len] =
      '\0';  // Добавление нулевого символа в конец новой строки

  return (void *)new_str;
}
