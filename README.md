# liteFilters
Легкая C++ библиотека с базовыми алгоритмами фильтрации сигналов для встраиваемых систем (Arduino, ESP32, STM32 и др.)

- **Header-only**: не требует компиляции, достаточно подключить один заголовочный файл.
- **Без зависимостей**: чистый C++, не привязан к `Arduino.h`.
- **Шаблоны**: размер окна Moving Average задается при компиляции.
- **Защита от скачков**: корректная инициализация первого значения.
- **Namespace**: весь код обернут в пространство имен `lf`.

### Совместимость
Совместима со всеми микроконтроллерами (C++98/11 и выше).

## Содержание
- [Обзор фильтров](#intro)
- [Описание классов](#classes)
- [Использование](#usage)
- [Установка](#install)

<a id="intro"></a>
## Обзор фильтров
| Фильтр | Описание | Применение |
| :--- | :--- | :--- |
| `EMAFilter` | Экспоненциальное среднее | Плавные данные |
| `KalmanFilter` | Упрощенный Калман | Динамические системы с высоким шумом |
| `MedianFilter3` | Медиана по 3 точкам | Удаление резких выбросов |
| `MovingAverageFilter` | Скользящее среднее | Фильтрация данных средним арифметическим |

<a id="classes"></a>
## Описание классов
Все классы находятся в пространстве имен `lf`. Каждый из них имеет метод `float update(float input)`: принимает данные, возвращает отфильтрованное значение.

```cpp
lf::EMAFilter filter(float alpha); // Конструктор. Альфа задается от 0 до 1. Чем меньше, тем сильнее сглаживание.
float update(float input);
```

```cpp
lf::KalmanFilter filter(float q, float r); // Конструктор. q (шум процесса), r (шум измерения).
void setParameters(float q, float r);      // Задание параметров фильтра.
float update(float measurement);        

```

```cpp
lf::MedianFilter3 filter;    // Конструктор. Окно усреднения в 3 элемента.
float update(float input); // Возвращает медиану массива из 3-х значений
```

```cpp
lf::MovingAverageFilter<N> filter; // Конструктор. N - размер окна (uint8_t).
float update(float input);         // Возвращает среднее арифметическое окна.
```

<a id="usage"></a>
## Использование

Пример фильтрации зашумленных данных с аналогового пина:

```cpp
#include <liteFilters.h>

// Создаем фильтры
lf::MedianFilter3 med;           
lf::MovingAverageFilter<10> avg;  // Окно усреднения 10
lf::EMAFilter ema(0.1);           // Коэффициент сглаживания 0.1

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Имитируем получение зашумленных данных
  float rawVal = analogRead(34);
  
  // Применяем фильтры по цепочке и Ema отдельно
  float cleanMed = med.update(rawVal);
  float cleanAvg = avg.update(cleanMed);
  float cleanEma = ema.update(rawVal);
  
  // Вывод в Serial Plotter
  Serial.print(">"); Serial.print("Raw: ");
  Serial.println(rawVal, 3);
  Serial.print(">"); Serial.print("Median: ");
  Serial.println(cleanMed, 3);
  Serial.print(">"); Serial.print("Average: ");
  Serial.println(cleanAvg, 3);
  Serial.print(">"); Serial.print("Ema: ");
  Serial.println(cleanEma, 3);
  
  delay(10);
}
```

<a id="install"></a>
## Установка

### Способ 1: Как библиотеку Arduino
1. Скачать репозиторий [.zip архивом](https://github.com/stpntrsvv/liteFilters/archive/refs/heads/main.zip).
2. В Arduino IDE выбрать: *Скетч -> Подключить библиотеку -> Добавить .ZIP библиотеку...* и указать скачанный архив.
3. В коде использовать: `#include <liteFilters.h>`.

### Способ 2: Напрямую в проект
1. Скопировать файл `include/liteFilters.h` в папку со своим скетчем/проектом.
2. В коде использовать: `#include "liteFilters.h"`.
```