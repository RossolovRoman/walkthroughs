#ifndef _WIEGAND_H
#define _WIEGAND_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class WIEGAND {

public:
    WIEGAND(const int pinReadD0,
            const int pinReadD1,
            const int pinPubD0,
            const int pinPubD1,
            int countBitsRead,
            int countBitsPub);
    void beginReader();
    void beginPublisher();
    void readD0();
    void readD1();
    String Data2Str(int startBit, int endBit);
    bool allReadBits();
    bool allPub();
    void calcParityBits(int countBits);
    void dataPublisherOnDelay(int countBits);
    void dataPublisher(int countBits);

private:
    void clearData();
    unsigned long timeInterval(unsigned long time, unsigned long last_time);

    const int _pinReadD0;
    const int _pinReadD1;

    const int _pinPubD0;
    const int _pinPubD1;

    bool* _data;
    volatile int _bitCountMax;

    volatile int _bitCountReader;
    unsigned long _lastBitRead;

    volatile int _bitCountPublisher;
    unsigned long _lastBitPub;

    bool _pubData;
    bool _nextBit;
};

#endif
