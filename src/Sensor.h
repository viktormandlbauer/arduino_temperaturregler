#define DATA_PIN 12

int reset()
{
    uint8_t r;

    cli();
    pinMode(DATA_PIN, INPUT);
    sei();

    do
    {
        delayMicroseconds(2);
    } while (!digitalRead(DATA_PIN));

    // Master pulls low for 480us
    cli();
    digitalWrite(DATA_PIN, LOW);
    pinMode(DATA_PIN, OUTPUT);
    sei();
    delayMicroseconds(480);

    // Release and wait for slave to respond
    cli();
    pinMode(DATA_PIN, INPUT);
    delayMicroseconds(70);

    // Slave pulls low for 60 - 240us
    r = !digitalRead(DATA_PIN);
    sei();

    // Wait for Pullup
    delayMicroseconds(410);

    return r;
}

void write_1()
{

    cli();
    digitalWrite(DATA_PIN, LOW);
    pinMode(DATA_PIN, OUTPUT);
    delayMicroseconds(5);
    pinMode(DATA_PIN, INPUT);
    delayMicroseconds(80);
    sei();
}

void write_0()
{
    cli();
    digitalWrite(DATA_PIN, LOW);
    pinMode(DATA_PIN, OUTPUT);
    delayMicroseconds(80);
    pinMode(DATA_PIN, INPUT);
    delayMicroseconds(5);
    sei();
}

int read()
{
    cli();
    digitalWrite(DATA_PIN, LOW);
    pinMode(DATA_PIN, OUTPUT);
    delayMicroseconds(5);
    pinMode(DATA_PIN, INPUT);
    delayMicroseconds(15);
    sei();

    return digitalRead(DATA_PIN);
}

void send_byte(byte byte)
{
    for (int i = 0; i < 8; i++)
    {
        if (byte & (0x01 << i))
        {
            write_1();
        }
        else
        {
            write_0();
        }
    }
}

void send_64bitcode(byte *bytes)
{
    for (int i = 0; i < 8; i++)
    {
        send_byte(bytes[i]);
    }
}

void skip_rom()
{
    send_byte(0xCC);
}

void read_rom()
{
    send_byte(0x33);
}

void convertT()
{
    send_byte(0x44);
    while (!read())
        ;
}

void read_scratchpad()
{
    send_byte(0xBE);
}

void match_rom()
{
    send_byte(0x55);
}

byte *getROM()
{

    static byte rom[8] = {
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    int byte_array_index = 0;
    int mod_8;
    byte mask;

    reset();
    read_rom();

    for (int i = 0; i < 64; i++)
    {
        mod_8 = i % 8;

        if (mod_8 == 0)
        {
            byte_array_index++;
        }
        if (read())
        {
            mask = 0x1 << mod_8;
            rom[byte_array_index - 1] |= mask;
        }
    }

    return rom;
}

byte *getScratchpad()
{

    static byte scratchpad[9] = {
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    reset();
    skip_rom();
    read_scratchpad();

    int byte_array_index = 9;
    int mod_8;
    byte mask;

    for (int i = 0; i < 72; i++)
    {
        mod_8 = i % 8;

        if (mod_8 == 0)
        {
            byte_array_index--;
        }
        if (read())
        {
            mask = 0x1 << mod_8;
            scratchpad[byte_array_index] |= mask;
        }
    }
    return scratchpad;
}

byte *getScratchpad(byte *rom)
{

    static byte scratchpad[9] = {
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    reset();
    match_rom();
    send_64bitcode(rom);
    read_scratchpad();

    int byte_array_index = 0;
    int mod_8;
    byte mask;

    for (int i = 0; i < 72; i++)
    {
        mod_8 = i % 8;

        if (mod_8 == 0)
        {
            byte_array_index++;
        }
        if (read())
        {
            mask = 0x1 << mod_8;
            scratchpad[byte_array_index - 1] |= mask;
        }
    }
    return scratchpad;
}

byte read_byte()
{
    byte read_byte;
    for (int i = 0; i < 8; i++)
    {
        if (read())
        {
            read_byte |= 0x01 << i;
        }
    }
    return read_byte;
}

float getTemperature()
{

    float temperatur = 0;

    reset();
    skip_rom();
    read_scratchpad();

    byte lsb = read_byte();
    byte msb = read_byte();

    uint16_t two_bytes = (msb << 8) | lsb;

    // Negative Temperatur
    if (msb & 0b111100)
    {
        two_bytes = two_bytes xor 0xFFF0;
        for (int i = -4; i < 6; i++)
        {
            if (two_bytes & (0x01 << (i + 4)))
            {
                if (i >= 0)
                {
                    temperatur += pow(2, i);
                }
                else
                {
                    temperatur -= pow(2, i);
                }
            }
        }
        reset();
        return (temperatur + 1) * -1;
    }

    // Positive Temperatur
    for (int i = -4; i < 6; i++)
    {
        if (two_bytes & (0x01 << (i + 4)))
        {
            temperatur += pow(2, i);
        }
    }
    reset();
    return temperatur;
}
