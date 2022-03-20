#include <Arduino.h>
#include <Sensor.h>
#include <KeyPad.h>
#include <Display.h>

void setup()
{
    Serial.begin(9600);

    //initKeys()

    pinMode(DIN_pin, OUTPUT);
    pinMode(CLOCK_pin, OUTPUT);
    pinMode(LOAD_pin, OUTPUT);

    // Aktiviert alle Reihen
    init(max7219_REG_scanLimit, 0x07); // set to scan all row

    // Kein decoding
    init(max7219_REG_decodeMode, 0x00);

    // Kein shutdown mode
    init(max7219_REG_shutdown, 0x01);

    // Kein Testmodus
    init(max7219_REG_displayTest, 0x00);

    // Setzt den Display zurück
    clear_screen();
}

/**
 * Input Mode wird mit "*" gestartet und gibt die eingaben auf dem Display aus.
 * Ist gekennzeichnet durch ein "größer als"-Zeichen: ">"
 **/

uint8_t input_position = 0;
uint8_t mode = 0;
float desired_temperature = 0;
float saved_temperatures[4] = {0, 0, 0, 0};

void input_mode(uint8_t key)
{
    if (key == 13)
    {
        input_position = 0;
        desired_temperature = 0;
        clear_screen();
        overlay(input_mode_sign, 0);
        draw_matrix();
    }
    else
    {
        if (input_position > 2)
        {
            desired_temperature = 0;
            input_mode(13);
        }
        desired_temperature += (key * pow(10, 1 - input_position));
        setTemperature(key, input_position);
        draw_matrix();
        input_position++;
    }
}

float last_temp = 0;

void loop()
{

    // Lese Eingabe
    int8_t key = getKey();
    if (key != -1)
    {
        // Starte "Input Mode" mit "*" -> Taste 13
        if (((mode == 1) & (key < 10)) | (key == 13))
        {
            // "Modus 1" -> Eingabe
            mode = 1;
            input_mode(key);
        }
        else if (key == 14)
        {
            Serial.print(desired_temperature);
            mode = 0;
        }
    }

    // "Modus 0" -> Temperaturanzeige
    if (mode == 0)
    {
        set_display_intensity();
        reset();
        delay(10);
        skip_rom();
        delay(10);
        convertT();
        delay(10);
        float temp = getTemperature();
        if (temp != last_temp)
        {
            last_temp = temp;
            clear_array();
            ShowTemperature(temp);
            draw_matrix();
        }
    }
}