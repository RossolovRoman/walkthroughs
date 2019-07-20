#include <Wiegand.h>

#define COUNT_BITS_READ    37    // Количество считываемых бит кода
#define COUNT_BITS_PUB     34    // Количество публикуемых бит кода

#define READER_A_D0      PB8  // Считыватель A линия D0
#define READER_A_D1      PB9  // Считыватель A линия D1
#define PUBLISHER_A_D0   PA1  // Вывод A линия D0
#define PUBLISHER_A_D1   PA0  // Вывод A линия D1

#define READER_B_D0      PB6  // Считыватель B линия D0
#define READER_B_D1      PB7  // Считыватель B линия D1
#define PUBLISHER_B_D0   PA3  // Вывод B линия D0
#define PUBLISHER_B_D1   PA2  // Вывод B линия D1

WIEGAND wgA(READER_A_D0, READER_A_D1, PUBLISHER_A_D0, PUBLISHER_A_D1, COUNT_BITS_READ, COUNT_BITS_PUB);
WIEGAND wgB(READER_B_D0, READER_B_D1, PUBLISHER_B_D0, PUBLISHER_B_D1, COUNT_BITS_READ, COUNT_BITS_PUB);

void setup() {
    wgA.beginReader();
    attachInterrupt(READER_A_D0, readAD0, FALLING);
    attachInterrupt(READER_A_D1, readAD1, FALLING);
    wgA.beginPublisher();

    wgB.beginReader();
    attachInterrupt(READER_B_D0, readBD0, FALLING);
    attachInterrupt(READER_B_D1, readBD1, FALLING);
    wgB.beginPublisher();
}

void loop() {
    if (wgA.allReadBits())
    {
        wgA.calcParityBits(COUNT_BITS_PUB);
    }
    if (wgB.allReadBits())
    {
        wgB.calcParityBits(COUNT_BITS_PUB);
    }


    if (wgA.allPub())
    {
        wgA.dataPublisher(COUNT_BITS_PUB);
    }
    if (wgB.allPub())
    {
        wgB.dataPublisher(COUNT_BITS_PUB);
    }
}

void readAD0()
{
    wgA.readD0();
}

void readAD1()
{
    wgA.readD1();
}

void readBD0()
{
    wgB.readD0();
}

void readBD1()
{
    wgB.readD1();
}
