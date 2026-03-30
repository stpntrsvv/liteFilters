#ifndef LITE_FILTERS_H
#define LITE_FILTERS_H

#include <stdint.h>

namespace lf {
class EMAFilter {
private:
    float _alpha;
    float _currentValue;
    bool _isInitialized;

public:
    EMAFilter(float alpha) : _alpha(alpha), _currentValue(0), _isInitialized(false) {}

    inline float update(float input) {
        if (!_isInitialized) {
            _currentValue = input;
            _isInitialized = true;
            return _currentValue;
        }
        _currentValue = (_alpha * input) + (1.0f - _alpha) * _currentValue;
        return _currentValue;
    }
};

class KalmanFilter {
private:
    float _q; 
    float _r; 
    float _x; 
    float _p; 
    float _k; 

public:
    KalmanFilter(float q, float r) : _q(q), _r(r), _x(0), _p(1.0f) {}

    inline float update(float measurement) {
        _p = _p + _q;

        _k = _p / (_p + _r);
        _x = _x + _k * (measurement - _x);
        _p = (1.0f - _k) * _p;

        return _x;
    }

    void setParameters(float q, float r) {
        _q = q;
        _r = r;
    }
};

class MedianFilter3 {
private:
    float _buffer[3];
    uint8_t _idx;
    bool _isInitialized;

public:
    MedianFilter3() : _idx(0), _isInitialized(false) {
        _buffer[0] = 0; _buffer[1] = 0; _buffer[2] = 0;
    }

    inline float update(float input) {
        if (!_isInitialized) {
            _buffer[0] = _buffer[1] = _buffer[2] = input;
            _isInitialized = true;
            return input;
        }

        // Заполняем кольцевой буфер
        _buffer[_idx] = input;
        _idx = (_idx + 1) % 3;

        // Копируем для сортировки
        float a = _buffer[0];
        float b = _buffer[1];
        float c = _buffer[2];

        if (a > b) { float t = a; a = b; b = t; }
        if (b > c) { b = c; }
        if (a > b) { b = a; }

        return b; // Возвращаем средний элемент
    }
};

template <uint8_t N>
class MovingAverageFilter {
    private:
        float _sum;
        float _buffer[N];
        uint8_t _idx;
        bool _isInitialized;
    public:
        MovingAverageFilter() : _sum(0), _idx(0), _isInitialized(false) {
            for (int i = 0; i < N; i++) _buffer[i] = 0;
        }
    
        inline float update(float input) {
            if (!_isInitialized) {
                for (int i = 0; i < N; i++) _buffer[i] = input;
                _sum = input * N;
                _isInitialized = true;
                return input;
            }

            // Удаляем старое значение из суммы, добавляем новое
            _sum -= _buffer[_idx];
            _buffer[_idx] = input;
            _sum += _buffer[_idx];

            // Обновляем индекс
            _idx = (_idx + 1) % N;

            return _sum / N; // Возвращаем среднее
        }

};
}

#endif