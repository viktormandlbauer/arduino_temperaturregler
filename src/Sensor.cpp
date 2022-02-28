
#include <Arduino.h>

void setup()
{
    Serial.begin(9600);
}

int reset()
{
    uint8_t r;
    uint8_t retries = 125;

    cli();
    pinMode(10, INPUT);
    sei();

    do
    {
        if (--retries == 0)
            return 1;
        delayMicroseconds(2);
    } while (!digitalRead(10));

    // Master pulls low for 480us
    cli();
    digitalWrite(10, LOW);
    pinMode(10, OUTPUT);
    sei();
    delayMicroseconds(480);

    // Release and wait for slave to respond
    cli();
    pinMode(10, INPUT);
    delayMicroseconds(70);

    // Slave pulls low for 60 - 240us
    r = !digitalRead(10);
    sei();

    // Wait for Pullup
    delayMicroseconds(410);

    return r;
}

void write_1()
{
    digitalWrite(10, LOW);
    pinMode(10, OUTPUT);
    pinMode(10, INPUT);
    delayMicroseconds(70);
}

void write_0()
{
    digitalWrite(10, LOW);
    pinMode(10, OUTPUT);
    delayMicroseconds(70);
}

void skip_rom()
{
    write_0();
    write_0();
    write_1();
    write_1();

    write_0();
    write_0();
    write_1();
    write_1();
}

void read_scratchpad()
{
    // BE

    write_0();
    write_1();
    write_1();
    write_1();

    write_1();
    write_1();
    write_0();
    write_1();
}

void loop()
{
    reset();
    skip_rom();
    read_scratchpad();

    pinMode(10, INPUT);
    for (int i = 0; i <= 64; i++)
    {
        Serial.print(digitalRead(10));
        delayMicroseconds(60);
    }

    delay(1000);
}