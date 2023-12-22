#include "s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  flag flags = {0};  // Структура для хранения флагов форматирования
  unsigned int cnt_symbol = 0;  // счетчик напечатаных символов
  va_list ptr;  // Инициализация списка аргументов переменной длины
  va_start(ptr, format);  // Начало обработки переменных аргументов
  while (*format) {
    if (*format != '%') {
      *str = *format;
      str++;  // двигаемся на один символ вперед в буферной строке
      format++;  // двигаемся на один символ вперед в форматной строке
      cnt_symbol++;  // увеличиваем счечик записаных символов
    } else {
      *str = '\0';
      format++;
      format = flags_parser(
          format, &flags);  // находим наши флаги, ширину, длинну и точность
      format = specificator_parser(format, &flags);  // парсим спецификаторы
      if (format) {
        if (flags.specificator ==
            '%') {  // если после % опять идет % то печатаем %
          *str = '%';
          str++;
          cnt_symbol++;
        } else
          str = run_specificator(&str, &format, flags, &ptr, &cnt_symbol);
      }
    }
    flags_clean(&flags);  // зануляем флаги перед следующим парсингом
  }
  va_end(ptr);
  return cnt_symbol;
}

const char *flags_parser(const char *format_str, flag *flags) {
  while (*format_str == '-' || *format_str == '+' || *format_str == ' ' ||
         *format_str == '#' ||
         *format_str == '0') {  // если находим какой-то из флагов, записываем
                                // их в структуру флагов и идем дальше
    switch (*format_str) {
      case '-':
        flags->minus_flag = 1;
        break;
      case '+':
        flags->plus_flag = 1;
        break;
      case '0':
        flags->zero_flag = 1;
        break;
      case '#':
        flags->sharp_flag = 1;
        break;
      default:
        flags->space_flag = 1;
        break;
    }
    format_str++;
  }

  while (*format_str >= '0' &&
         *format_str <= '9')  // если нашли ширину (ширна может быть как
                              // однозначным, так и многозначным числом) занесли
                              // в флаг ширины значение ширины и пошли дальше
  {
    flags->width_value *= 10;
    flags->width_value += *format_str - 48;
    format_str++;
  }
  if (*format_str == '.') {
    format_str++;
    flags->precision_flag = 1;
    while (*format_str >= '0' && *format_str <= '9') {
      flags->precision_value *= 10;
      flags->precision_value += *format_str - 48;
      format_str++;
    }
  }
  if (*format_str == 'h' || *format_str == 'l' ||
      *format_str ==
          'L')  // если нашли длинну, пометили что нашли и какую и пошли дальше
  {
    flags->length_value = *format_str;
    format_str++;
  }

  return format_str;
}

const char *specificator_parser(const char *format_str, flag *flags) {
  int find_spec = 1;
  if (!(*format_str >= 48 || *format_str <= 57)) {
    switch (*format_str) {
      case 'c':
      case 'd':
      case 'i':
      case 'f':
      case 's':
      case 'o':
      case 'u':
      case 'x':
      case 'X':
      case 'e':
      case 'E':
      case 'g':
      case 'G':
      case 'p':
      case 'n':
        break;
      default:
        find_spec = 0;
        break;
    }
  }
  flags->specificator = *format_str;
  format_str++;
  if (find_spec == 0) {  // если не нашли спецификатор, то вернули 0
    format_str = S21_NULL;
  }
  return format_str;
}

char *run_specificator(char **str, const char **format, flag flags,
                       va_list *ptr, unsigned int *count) {
  switch (flags.specificator) {
    case 'c':
      *str = spec_char(str, format, flags, ptr, count);

      break;
    case 'd':
    case 'i':
      *str = spec_int(str, flags, ptr, count);
      break;
    case 'f':
      *str = spec_double(str, flags, ptr, count);
      break;
    case 's':
      *str = spec_string(str, flags, ptr, count);
      break;
    case 'o':
    case 'u':
    case 'x':
    case 'X':
      *str = spec_uint(str, flags, ptr, count);
      break;
  }

  return *str;
}

char *spec_double(char **str, flag flags, va_list *ptr, unsigned int *count) {
  long double tmp, tmp1;
  int i = 0, length_value = 0;
  tmp = tmp1 = va_arg(*ptr, double);

  char number[666] = {0};  // массив, в который будем писать число
  // оставляем первую ячейку на случай удлиннения числа
  char *number_1 = &(number[1]);
  // если флаг, то используем длину или дефолт 6
  int precision_value = flags.precision_flag ? flags.precision_value : 6;
  // приводим занчение к строке
  double_to_str(tmp, tmp1, number_1, &length_value);

  rounding_double(length_value, precision_value, number_1);
  if (number_1[s21_strlen(number_1) - 1] == '.') {
    number_1[s21_strlen(number_1) - 1] = 0;
  }
  // Если имеется знак "-", возвращаем исходную строку
  if (number_1[-1]) {
    number_1 = number;
  }
  int size = ((unsigned int)(flags.width_value + 2) > (s21_strlen(number_1) + 2)
                  ? (unsigned int)(flags.width_value + 2)
                  : (s21_strlen(number_1) + 2));
  char *result = calloc(size + 1, sizeof(char));
  // массив для отформатированного числа
  for (;
       !flags.minus_flag && (flags.width_value - (int)s21_strlen(number_1) > 0);
       i++, flags.width_value--) {
    if (flags.zero_flag)
      // заполняем ширину нулями
      result[i] = '0';
    else
      // или пробелами
      result[i] = ' ';
  }
  if (tmp1 < 0.0) {
    negativeSign(result, i);
  } else {
    positiveSign(flags, result, i);
  }
  // копируем в финальную строку вместе с нулевой ячейкой
  if (number[0] != '\0') {
    s21_strcat(result, number);
  } else
    s21_strcat(result, number_1);
  // или без смотря, появилась ли там цифра после округления
  if (flags.minus_flag) {
    for (; (int)s21_strlen(result) < flags.width_value;) {
      // ширина
      s21_strcat(result, " ");
    }
  }
  int add = s21_strlen(result);
  s21_strcat(*str, result);
  *count += add;
  free(result);

  return *str + add;
}

char *spec_char(char **str, const char **format, flag flags, va_list *ptr,
                unsigned int *count) {
  wchar_t symbol =
      va_arg(*ptr, wchar_t);  // принимаем аргумент за символ wchar_t - тип
                              // данных для представления широких символов

  if (flags.length_value !=
      'l')  // если флаг длинны l выключен, то символ имеет обычный чар размер
  {
    symbol = (char)symbol;
  }
  if (flags.length_value == 'l' &&
      symbol > 127)  // если у нас широкий символ и он не стандартной аски то
                     // зануляем форматированную строку
  {
    (**str) = '\0';
    *format = 0;
  } else {
    if (!flags.minus_flag &&
        (flags.width_value - 1 >
         0))  // если выравнивание по левой стороне выключено
              // и есть ширина, печатаем пробелы для ширины
    {
      for (; flags.width_value - 1 > 0;
           (*str)++, flags.width_value--, (*count)++) {
        (**str) = ' ';
      }
    }

    (**str) = symbol;  // как напчетали все пробелы ширины печатаем символ
    (*str)++;          // двигаемся дальше по буферу
    (*count)++;  // увеличиваем счетчик напечатанных символов

    if (flags.minus_flag) {  // если включено выравнивание слева(минус) то
                             // печатаем
                             // пробелы после символа
      for (; (flags.width_value - 1 > 0);
           (*str)++, flags.width_value--, (*count)++) {
        (**str) = ' ';
      }
    }

    **str = '\0';  // заканчиваем строку
  }
  return *str;
}

char *spec_string(char **str, flag flags, va_list *ptr, unsigned int *count) {
  char *string = va_arg(*ptr, char *);  // получаем указатель на нашу строку
  int length_value = 0;  // наша длинна строки
  // получаем нашу длину строки, если указатель пустой, то длина по умолчанию
  // 6, иначе измеряем стрленом нашу строку
  if (string == S21_NULL) {
    length_value = 6;  // для записи нулл символа (S21_NULL)
  } else {
    length_value = s21_strlen(string);
  }
  // если включена точность, то наша длина равна нашей точности
  if (flags.precision_flag && flags.precision_value < length_value) {
    length_value = flags.precision_value;
  }
  // обрабатываем случай если пустой указатель и точность задана
  if (flags.precision_flag && string == S21_NULL && flags.precision_value < 6) {
    length_value = 0;  // because original
  }

  int n = 0;  // размер нашей результирующей строки
  if (length_value > flags.width_value) {
    n = length_value;
  } else {
    n = flags.width_value;
  }

  char *result = calloc(n, sizeof(char));
  int i = 0;
  if (!flags.minus_flag && (flags.width_value - length_value > 0)) {
    for (; (flags.width_value - length_value > 0); i++, flags.width_value--) {
      if (flags.zero_flag)
        result[i] = '0';
      else
        result[i] = ' ';
    }
  }
  // копируем строку в результирующую строку
  if (string != S21_NULL) {
    for (int j = 0; j < length_value; j++, i++) {
      result[i] = string[j];
    }
  } else if (length_value != 0) {
    char *string = "(null)";
    for (int wq = 0; wq < 7; wq++, i++) result[i] = string[wq];
  }
  result[i] = '\0';

  // заполняем ширину пробелами, если необходимо (для флага minus_flag)
  if (flags.minus_flag) {
    for (; s21_strlen(result) < (s21_size_t)flags.width_value; i++) {
      result[i] = ' ';
    }
  }
  // добавляем результирующую строку к выходной строке
  s21_strcat(*str, result);
  *str += s21_strlen(result);  // обновляем указатель на выходную строку
  *count += s21_strlen(result);  // обновляем количество символов
  free(result);  // освобождаем память нашей результирующей строки

  return *str;
}

char *spec_int(char **str, flag flags, va_list *ptr, unsigned int *count) {
  long int x =
      va_arg(*ptr, long int);  // Извлекаем аргумент, который нужно напечатать
  int i = 0;
  int size;
  char *array =
      calloc(21, sizeof(char));  // Выделяем память для временного массива
  char *tmp = array;  // Сохраняем указатель на начало массива

  if (flags.length_value == 'h') {
    x = (short)x;  // Изменяем тип данных в зависимости от параметра длины ('h')
  } else if (flags.length_value == 0) {
    x = (int)x;  // Изменяем тип данных в зависимости от параметра длины (по
                 // умолчанию)
  }

  itoa_function(x, array);  // Преобразуем число в строку

  if (x == 0 && flags.precision_flag && !flags.precision_value) {
    array[0] = '\0';
    // Особенность работы с нулём: если число 0 и указана точность,
    // но она равна 0, то заменяем первый символ на '\0'
  }

  if (array[0] == '-') {
    array++;  // Пропускаем минус, чтобы не учитывать его при дальнейшей
              // обработке
  }

  // Определяем размер выходной строки в зависимости от заданной ширины и
  // точности
  if (21 > (flags.precision_value > flags.width_value ? flags.precision_value
                                                      : flags.width_value)) {
    size = 21 + 2;
    // Учитываем максимальную ширину числа (21) и дополнительные символы
  } else if (flags.precision_value > flags.width_value) {
    size = flags.precision_value + 2;
    // Учитываем точность числа и дополнительные символы
  } else {
    size = flags.width_value + 2;
    // Учитываем ширину числа и дополнительные символы
  }

  char *result = calloc(size, sizeof(char));
  // Выделяем память для результирующей строки с нужным форматированием
  widthHelper(result, flags, &i, s21_strlen(array));
  // Обрабатываем ширину числа

  if (x < 0) {
    // Добавляем знак минуса, если необходимо
    negativeSign(result, i);
  } else {
    // Добавляем знак плюса, если необходимо
    positiveSign(flags, result, i);
  }

  for (; (flags.precision_value - (int)s21_strlen(array)) > 0;
       i++, flags.precision_value--) {
    // Дополняем число нулями до нужной точности
    s21_strcat(result, "0");
  }

  // Вставляем число в массив, где произведено правильное форматирование
  s21_strcat(result, array);

  if (flags.minus_flag) {
    while ((int)s21_strlen(result) < flags.width_value) {
      // Если есть флаг минус, добавляем пробелы после числа для заданной ширины
      s21_strcat(result, " ");
    }
  }

  // Добавляем отформатированное число в итоговую строку
  s21_strcat(*str, result);
  // Подсчитываем, сколько символов было добавлено
  unsigned int check = s21_strlen(result);
  *count += check;  // Увеличиваем счетчик напечатанных символов
  free(result);
  free(tmp);
  return *str + check;
  // Возвращаем указатель на текущую позицию в итоговой строке
}

char *spec_uint(char **str, flag flags, va_list *ptr, unsigned int *count) {
  // Извлекаем беззнаковое целое число из списка аргументов переменной длины
  unsigned long int x = va_arg(*ptr, unsigned long int);

  // Инициализируем переменные

  char num[24] = {0};  // Массив для хранения числа

  // Приводим число к нужному типу данных в зависимости от указанной длины
  if (flags.length_value == 'h') {
    x = (unsigned short)x;
  } else if (flags.length_value == 0) {
    x = (unsigned int)x;
  }

  // Преобразуем число в строку с указанной системой счисления
  if (flags.specificator == 'o') {
    // 8-чная система
    uitoa_function(x, num, 8);
  } else if (flags.specificator == 'u') {
    // 10-чная система
    uitoa_function(x, num, 10);
  } else {
    // 16-чная система
    uitoa_function(x, num, 16);
  }

  if (flags.specificator == 'X') {
    upper(num);
  }

  // Обрабатываем случай, когда число равно 0 и указана точность, но она равна 0
  if (x == 0 && flags.precision_flag && !flags.precision_value) {
    num[0] = '0';
  }

  int i = 0;
  int size = 24;  // место под инт
  if (24 > (flags.precision_value > flags.width_value ? flags.precision_value
                                                      : flags.width_value)) {
    size = 24 + 2;
  } else if (flags.precision_value > flags.width_value) {
    size = flags.precision_value + 2;
  } else {
    size = flags.width_value + 2;
  }

  // Выделяем память для результирующей строки с нужным форматированием
  char *result = calloc(size, sizeof(char));

  // Обрабатываем ширину числа
  widthHelper(result, flags, &i, s21_strlen(num));

  // Добавляем нули до нужной точности
  for (; flags.precision_value - (int)s21_strlen(num) > 0;
       i++, flags.precision_value--) {
    result[i] = '0';
  }
  result[i] = '\0';

  // Добавляем число в форматированную строку
  s21_strcat(result, num);

  // Если задан флаг выравнивания по левому краю (минус), добавляем пробелы
  // после числа для заданной ширины
  if (flags.minus_flag) {
    while ((int)s21_strlen(result) < flags.width_value) {
      s21_strcat(result, " ");
    }
  }

  // Добавляем форматированное число в итоговую строку
  s21_strcat(*str, result);

  // Обновляем счетчик напечатанных символов
  unsigned int add = s21_strlen(result);
  *count += s21_strlen(result);

  // Освобождаем выделенную память
  free(result);

  // Возвращаем указатель на текущую позицию в итоговой строке
  return *str + add;
}

void flags_clean(flag *flags) {
  flags->length_value = 0;
  flags->width_value = 0;
  flags->minus_flag = 0;
  flags->plus_flag = 0;
  flags->precision_flag = 0;
  flags->precision_value = 0;
  flags->sharp_flag = 0;
  flags->space_flag = 0;
  flags->specificator = 0;
  flags->zero_flag = 0;
}

char *reverse(char s[]) {
  char tmp;
  int i, j;
  int length_value = s21_strlen(s);  // Получаем длину строки

  // Используем два указателя, один идет с начала строки, другой с конца
  // Переставляем символы до тех пор, пока не достигнут середины строки
  for (i = 0, j = length_value - 1; i < j; i++, j--) {
    tmp = s[i];
    s[i] = s[j];
    s[j] = tmp;
  }
  return s;
}

char *itoa_function(long long int value, char *result) {
  int n = labs(value);  // Получаем абсолютное значение числа
  int i = 0;

  while (n) {
    int r = n % 10;  // Вычисляем остаток от деления на 10
    result[i++] = 48 + r;  // Добавляем цифру от '0' до '9'
    n /= 10;               // Делаем деление на 10
  }

  if (i == 0) {
    result[i++] = '0';  // Если число равно 0, добавляем символ '0'
  }

  if (value < 0) {
    result[i++] = '-';  // Если число отрицательное, добавляем символ '-'
  }

  result[i] = '\0';  // Добавляем завершающий нулевой символ

  return reverse(result);  // Возвращаем перевернутую строку
}

char *uitoa_function(long long unsigned value, char *result, int base) {
  long long unsigned n = value;
  int i = 0;

  while (n) {
    int r = n % base;

    if (r >= 10) {
      // Добавляем символ от 'a' до 'f' для значений больше 9
      result[i++] = 97 + (r - 10);
    } else {
      result[i++] = 48 + r;  // Добавляем цифру от '0' до '9'
    }
    n /= base;
  }

  if (i == 0) {
    result[i++] = '0';
    // Если число равно 0, добавляем нулевой символ  \\ was \0
  }

  result[i] = '\0';  // Добавляем завершающий нулевой символ

  return reverse(result);  // Возвращаем перевернутую строку
}

void widthHelper(char *result, flag flags, int *i, int strlength) {
  while (!flags.minus_flag &&
         (flags.width_value - (flags.precision_value > strlength
                                   ? flags.precision_value
                                   : strlength) >
          0)) {
    if (!flags.precision_flag && flags.zero_flag)
      result[(*i)] = '0';  // Заполняем отступ нулями, если флаг zero_flag
                           // установлен и не используется точность
    else
      result[(*i)] = ' ';  // Заполняем отступ пробелами
    (*i)++;
    flags.width_value--;
  }
}

void negativeSign(char *str, int i) {
  int n = 0;
  if (!(str[0] == '0' || i == 0)) {
    n = i - 1;
  }
  str[n] = '-';
}

void positiveSign(flag flags, char *str, int i) {
  int n = 0;
  if (!(str[0] == '0' || i == 0)) n = i - 1;
  if (flags.plus_flag) {
    str[n] = '+';
  } else if (flags.space_flag) {
    str[n] = ' ';
  }
}

void double_to_str(long double tmp, long double tmp1, char *number,
                   int *length_value) {
  if (tmp < 0.0) tmp = -1 * tmp;
  tmp1 = (tmp - fmodl(tmp, 1));  // целая часть
  char *kind_of_int = number;

  if (tmp1 == 0.0)
    number[0] = '0';
  else {
    itoa_function(tmp1, kind_of_int);
  }
  s21_strcat(number, ".");  // после печати целой части печатаем точку

  *length_value = s21_strlen(number);  // отмечаем длину получившегося числа
  // берём дробную часть исходного числа (по модулю)
  tmp1 = tmp = fabsl(fmodl(tmp, 1));
  char digit[2] = {'0', '\0'};  // будем использовать эту строку для стркатов
  for (int z = 0; z < 50; z++) {  // максимально печатаемая точность до 50
    tmp1 = tmp = tmp * 10;  // первое число после точки ставим перед точкой
    tmp1 -= fmodl(tmp1, 1);  // убираем дробную часть числа, так что останется
    // только цифра перед точкой
    digit[0] = tmp1 + '0';  // записываем эту цифру в строку для стркатов
    s21_strcat(number, digit);
    // добавляем её в строку, в которую записываем исходное число
    tmp1 = tmp = fmodl(tmp, 1);
    // записываем сюда десятичную часть исходного числа
    // после того, как забрали из неё ещё один символ
  }
}

void rounding_double(int length_value, int precision_value, char *number) {
  // округляем десятичную дробь в соотвествии с нужной точностью
  unsigned int strlength = s21_strlen(number);  // длина строки
  int hvost = length_value + precision_value;
  if ((unsigned int)(hvost) < strlength) {  // если строка должна быть короче
    int count = 1;
    int dot_flag = 0;
    if (number[hvost] >= '5') {  // если предыдущую цифру нужно менять
      while (number[hvost - count] == '9' || number[hvost - count] == '.') {
        // пока встречаем 9-ки или точку
        if (number[hvost - count] == '.')
          dot_flag = 1;  // отмечаем точку
        else
          number[hvost - count] = '0';  // зануляем число
        count++;  // двигаемся дальше влево
      }
      // если мы оказались в позиции, где числа раньше не было
      if (number[hvost - count] == '\0')
        number[hvost - count] = '1';  // печатаем туда единицу
      else
        number[hvost - count] += 1;  // иначе число станет больше на единицу
    }
    if (precision_value == 0)
      dot_flag = 1;  // если нужна нулевая точность, тоже отметим точку
    if (!dot_flag) number[hvost] = '\0';
    // если точки не было, терминирующий 0 после числа
    else {  // если dot_flag = 1, то цифры числа после точки надо отрезать при
      // округлении
      int dot_location = 0;
      for (; number[dot_location] != '.'; dot_location++)
        ;  // находим точку
      if (precision_value == 0)
        number[dot_location] = '\0';  // если её надо оставить - оставляем
      else
        number[dot_location + precision_value + 1] =
            0;  // если нет - отрезаем вместе с остатком числа
    }
    for (unsigned int str_len_new = s21_strlen(number); str_len_new < strlength;
         str_len_new++)
      number[str_len_new] = 0;  // чистим остаток строки
  }
}

void upper(char *str) {
  if (str) {  // если память выделилась
    for (int i = 0; str[i]; i++) {
      if (97 <= str[i] &&
          str[i] <= 122) {  // проверям если символ в нижнем регистре
        str[i] = str[i] - 32;  // меняем регистр
      }
    }
  }
}
