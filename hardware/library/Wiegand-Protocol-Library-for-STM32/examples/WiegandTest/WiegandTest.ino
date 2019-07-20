#include <Wiegand.h>

#define COUNT_BITS_READ    37    // Количество считываемых бит кода
#define COUNT_BITS_PUB     34    // Количество публикуемых бит кода

#define READER_D0      PB8  // Считыватель линия D0
#define READER_D1      PB9  // Считыватель линия D1
#define PUBLISHER_D0   PB1  // Вывод линия D0
#define PUBLISHER_D1   PB0  // Вывод линия D1

WIEGAND wg(READER_D0, READER_D1, PUBLISHER_D0, PUBLISHER_D1, COUNT_BITS_READ, COUNT_BITS_PUB);

void setup() {
  Serial.begin(112500);

  wg.beginReader();
  attachInterrupt(READER_D0, readD0, FALLING);
  attachInterrupt(READER_D1, readD1, FALLING);
  wg.beginPublisher();
}

void loop() {
  if (wg.allReadBits())
  {
    Serial.print("Read Wiegand Binary:  ");
    Serial.println(wg.Data2Str(0, COUNT_BITS_READ));

    wg.calcParityBits(COUNT_BITS_PUB);

    Serial.print("Write Wiegand Binary: ");
    for (int i = COUNT_BITS_PUB; i < COUNT_BITS_READ; i++)
    {
      Serial.print(" ");
    }
    Serial.println(wg.Data2Str(COUNT_BITS_READ - COUNT_BITS_PUB, COUNT_BITS_READ));
  }

  if (wg.allPub())
  {
    wg.dataPublisher(COUNT_BITS_PUB);

    //Serial.print("Status Write Wiegand Binary: ");
    //Serial.println(wg.allPub());
  }
}

void readD0()
{
    wg.readD0();
}

void readD1()
{
    wg.readD1();
}
