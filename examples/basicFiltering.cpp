#include "liteFilters.h"
#include <Arduino.h>

lf::MedianFilter3 medF;
lf::MovingAverageFilter<10> avgF;
lf::EMAFilter emaF(0.1f);

void setup() {
    Serial.begin(115200);
    Serial.println("Reading, Median, Average, EMA");
}

void loop() {
    // Имитируем получение данных для фильтрации
    float rawValue = analogRead(34);

    // Пропускаем сигнал через цепочку фильтров
    // Сначала медианный фильтр, потом усредняем
    float cleanMedian = medF.update(rawValue);
    float cleanAvg    = avgF.update(cleanMedian);
    float cleanEma    = emaF.update(cleanMedian);

    // Блок вывода результатов
    Serial.print(rawValue);    Serial.print(",");
    Serial.print(cleanMedian); Serial.print(",");
    Serial.print(cleanAvg);    Serial.print(",");
    Serial.println(cleanEma);

    delay(10);
}