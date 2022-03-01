
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

    cli();
    digitalWrite(10, LOW);
    pinMode(10, OUTPUT);
    delayMicroseconds(5);
    pinMode(10, INPUT);
    delayMicroseconds(80);
    sei();
}

void write_0()
{
    cli();
    digitalWrite(10, LOW);
    pinMode(10, OUTPUT);
    delayMicroseconds(80);
    pinMode(10, INPUT);
    delayMicroseconds(5);
    sei();
}

int read(){
    cli();
    digitalWrite(10, LOW);
    pinMode(10, OUTPUT);
    delayMicroseconds(5);
    pinMode(10, INPUT);
    delayMicroseconds(10);
    sei();

    return digitalRead(10);
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

void convertT(){

    // 44
    write_0();
    write_0();
    write_1();
    write_0();

    write_0();
    write_0();
    write_1();
    write_0();

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

    byte scratchpad[9] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000};

    reset();
    skip_rom();
    read_scratchpad();
    
    int byte_array_index = 0;
    byte mod_8;
    byte mask;

    for (int i = 0; i < 72; i++)
    {
        mod_8 = i % 8;
        
        if(mod_8 == 0){
            byte_array_index ++;
        }
        if(read()){
            mask = 0x1 << (7 - mod_8);
            scratchpad[byte_array_index - 1] |= mask;
        }
    }

    for(int i = 0; i <= 8; i++){
        Serial.print(scratchpad[i], BIN);
        Serial.print("\n");
    }
    Serial.print("\n");

    delay(5000);
}