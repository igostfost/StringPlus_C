#include "s21_string.h"

/* s21_trim Возвращает новую строку, в которой удаляются все начальные и
 конечные вхождения набора заданных символов (trim_chars) из данной строки
 (src). В случае какой-либо ошибки следует вернуть значение S21_NULL */

/* right_side - вспомогательная функция которая убирает трим символы с конца */

void right_side(char *str_trim, const char *src, const char *trim_chars) {
  s21_size_t trim_lenght =
      s21_strlen(trim_chars);  // записываем длину строк трим
  s21_size_t str_trim_length = s21_strlen(str_trim);  // записываем длину строки
  for (int i = (int)str_trim_length - 1; i >= 0;
       i--) {            // движемся по строке от конца
    int flag_break = 0;  // флаг на случай выхода из цыкла
    for (s21_size_t j = 0; j < trim_lenght;
         j++) {  // на каждом символе строки проходимся по строке трим
      if (str_trim[i] == trim_chars[j]) {  // если втречается значение трим
        str_trim[i] =
            '\0';  // записываем окончание строки или освобождаем ячейку
        break;     // выходим из проверки по трим
      }
      if (j == trim_lenght - 1 && src[i] != trim_chars[trim_lenght]) {
        flag_break = 1;  // если дошли до границ флаг брейк
      }
    }
    if (flag_break) break;  // выход по флагу
  }
}

/* left_side - вспомогательная функция которая убирает трим символы с начала */

void left_side(char *str_trim, const char *src, const char *trim_chars) {
  s21_size_t trim_lenght =
      s21_strlen(trim_chars);  // записываем длину строк трим
  s21_size_t str_length = s21_strlen(src);  // записываем длину строки
  for (s21_size_t i = 0; i < str_length; i++) {  // движемся по строке от начала
    int flag_break = 0;  // флаг на случай выхода из цыкла
    for (s21_size_t j = 0; j < trim_lenght;
         j++) {  // на каждом символе строки проходимся по строке трим
      if (src[i] == trim_chars[j]) {  // если втречается значение трим
        s21_size_t counter = 0;
        while (str_trim[counter]) {  // пропускам занчения
          str_trim[counter] = str_trim[counter + 1];
          counter++;  // (стрираем трим занчения путем перезаписи на яцейку
                      // влево)
        }
        break;  // выходим из проверки по трим
      }
      if (j == trim_lenght - 1 && src[i] != trim_chars[trim_lenght]) {
        flag_break = 1;  // если дошли до границ флаг брейк
      }
    }
    if (flag_break) break;  // выход по флагу
  }
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *str_trim = S21_NULL;  // создаем указатель на будущую строку и зануляем
                              // на случай ошибок
  if (src) {  // если строка содержит чего-либо то выполняем
    char default_chars[10] =
        " \t\n\v\r\f\0";  // прописываем дефолтные значения трима
    if (trim_chars == S21_NULL || s21_strlen(trim_chars) == 0)
      trim_chars = default_chars;  // если трим не передан - используем дефолт
    str_trim =
        (char *)malloc((s21_strlen(src) + 10) *
                       sizeof(char));  // выделяем память под новую строку
    if (str_trim != S21_NULL) {  // если память выделилась
      s21_strcpy(str_trim,
                 (char *)src);  // переносим значение строки в новую строку
      left_side(str_trim, src, trim_chars);  // убираем трим слева
      right_side(str_trim, src, trim_chars);  // убираем трим справа
    }
  }
  return (void *)str_trim;  //(void *) возвращаем указатель, иначе нулл
}
