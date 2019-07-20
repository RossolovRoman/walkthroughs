#include "Wiegand.h"
#include <math.h>

#define RATE_IMPULSE      100   // Длительность импульса
#define RATE_LIMIT        500   // Задержка до обнуления сигнала
#define RATE_END          10000 // Задержка до обнуления сигнала

WIEGAND::WIEGAND(
        const int pinReadD0,
        const int pinReadD1,
        const int pinPubD0,
        const int pinPubD1,
        int countBitsRead,
        int countBitsPub)
    :
      _pinReadD0(pinReadD0),
      _pinReadD1(pinReadD1),
      _pinPubD0(pinPubD0),
      _pinPubD1(pinPubD1),
      _bitCountMax(countBitsRead),
      _bitCountReader(0),
      _bitCountPublisher(countBitsRead-countBitsPub),
      _pubData(false),
      _nextBit(true)

{
    //Инициализация массива
    _data = (bool *) malloc(_bitCountMax * sizeof(bool));
    clearData();
}

//Инициализация считывателя
void WIEGAND::beginReader()
{
    pinMode(_pinReadD0, INPUT_PULLDOWN);
    pinMode(_pinReadD1, INPUT_PULLDOWN);
}

//Инициализация публикатора
void WIEGAND::beginPublisher()
{
    pinMode(_pinPubD0, OUTPUT);
    pinMode(_pinPubD1, OUTPUT);
    digitalWrite(_pinPubD0, HIGH);
    digitalWrite(_pinPubD1, HIGH);
}

//Обработчик прерывания на линии D0
void WIEGAND::readD0()
{
    unsigned long t = timeInterval(micros(), _lastBitRead);
    if(t >= RATE_END)
    {
        _bitCountReader = 0;
        clearData();
    }
    _data[_bitCountReader] = 0;
    _bitCountReader++;
    _lastBitRead = micros();
}

//Обработчик прерывания на линии D1
void WIEGAND::readD1()
{
    unsigned long t = timeInterval(micros(), _lastBitRead);
    if(t >= RATE_END)
    {
        _bitCountReader = 0;
        clearData();
    }
    _data[_bitCountReader] = 1;
    _bitCountReader++;
    _lastBitRead = micros();
}

//Конвертирование считанного кода в строку
String WIEGAND::Data2Str(int startBit, int endBit)
{
    String data;
    for (int i = startBit; i < endBit; i++)
    {
        data+=_data[i];
    }
    return data;
}

//Очистить считанный код
void WIEGAND::clearData() {
    for (int i = 0; i < _bitCountMax; i++)
    {
        _data[i] = 0;
    }
}

//Подсчитать биты счетности
void WIEGAND::calcParityBits(int countBits)
{
    if(countBits <= _bitCountMax)
    {
        _data[_bitCountMax-countBits] = 0;
        _data[_bitCountMax-1] = 1;

        for (int i = _bitCountMax-countBits+1; i <= _bitCountMax-2; i++)
        {
            if (i <= _bitCountMax-round(countBits/2) && _data[i] == 1 && countBits%2 > 0 ||
                    i < _bitCountMax-(countBits/2) && _data[i] == 1 && countBits%2 == 0) {
                _data[_bitCountMax-countBits] = !_data[_bitCountMax-countBits];
            }
            if (i >= _bitCountMax-round(countBits/2) && _data[i] == 1) {
                _data[_bitCountMax-1] = !_data[_bitCountMax-1];
            }
        }
    }
}

//Опубликовать код
void WIEGAND::dataPublisher(int countBits)
{
    if (_nextBit == true)
    {
        switch (_data[_bitCountPublisher])
        {
        case 0:
            digitalWrite(_pinPubD0, LOW);
            break;
        case 1:
            digitalWrite(_pinPubD1, LOW);
            break;
        }
        _lastBitPub = micros();
        _nextBit = false;
    }

    unsigned long t = timeInterval(micros(), _lastBitPub);
    if (t >= RATE_IMPULSE) {
        digitalWrite(_pinPubD0, HIGH);
        digitalWrite(_pinPubD1, HIGH);
        _nextBit = true;
        _bitCountPublisher++;
    }


    if (_bitCountPublisher >= _bitCountMax)
    {
        clearData();
        _bitCountPublisher=_bitCountMax-countBits;
        _pubData = false;
    }
}

//Опубликовать код используя задержки
void WIEGAND::dataPublisherOnDelay(int countBits)
{
    for(int i = _bitCountMax - countBits; i < _bitCountMax; i++){
        switch (_data[i])
        {
        case 0:
            digitalWrite(_pinPubD0, LOW);
            break;
        case 1:
            digitalWrite(_pinPubD1, LOW);
            break;
        }
        delayMicroseconds(100);
        digitalWrite(_pinPubD0,HIGH);
        digitalWrite(_pinPubD1,HIGH);
    }
    clearData();
    _pubData = false;
}

//Считаны все биты?
bool WIEGAND::allReadBits() {
    unsigned long t = timeInterval(micros(), _lastBitRead);
    if (t >= RATE_LIMIT && _bitCountReader == _bitCountMax)
    {
        _pubData = true;
        _bitCountReader = 0;
        return true;
    }
    return false;
}

bool WIEGAND::allPub() {
    return _pubData;
}

unsigned long WIEGAND::timeInterval(unsigned long time, unsigned long last_time) {
    if(time >= last_time)
    {
        return time - last_time;
    }
    else
    {
        unsigned long max_unsigned_long = -1;
        return max_unsigned_long - last_time + time;
    }
}
