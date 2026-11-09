#include <Arduino.h>
#include <nrf.h>

// SenseCAP Solar Node P1
// Charger-only firmware

// SX1262 pins
#define LORA_NSS       D4
#define LORA_RESET     D2
#define LORA_BUSY      D3
#define LORA_DIO1      D1
#define LORA_RXEN      D5

static void disablePeripherals()
{
    // 1. Disable battery voltage divider
    // LOW = enabled, HIGH = disabled

    pinMode(VBAT_ENABLE, OUTPUT);
    digitalWrite(VBAT_ENABLE, HIGH);

    // 2. Disable SX1262 RF receiver path

    pinMode(LORA_RXEN, OUTPUT);
    digitalWrite(LORA_RXEN, LOW);

    // 3. Hold SX1262 in hardware reset

    pinMode(LORA_NSS, OUTPUT);
    digitalWrite(LORA_NSS, HIGH);

    pinMode(LORA_RESET, OUTPUT);
    digitalWrite(LORA_RESET, LOW);

    // 4. Disable radio interrupt

    pinMode(LORA_DIO1, INPUT_PULLDOWN);
    pinMode(LORA_BUSY, INPUT_PULLDOWN);

    // 5. SPI pins

    pinMode(D8, INPUT_PULLDOWN);
    pinMode(D9, INPUT_PULLDOWN);
    pinMode(D10, INPUT_PULLDOWN);

    // 6. Blue LED
    // Use the board-defined pin, do not redefine it.

    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, HIGH);

    delay(20);
}

static void enterSystemOff()
{
    // Enable internal DC/DC
    NRF_POWER->DCDCEN = 1;

    // Enter lowest power state
    __DSB();

    NRF_POWER->SYSTEMOFF = 1;

    __DSB();

    while (true)
    {
        __WFE();
    }
}

void setup()
{
    disablePeripherals();
    enterSystemOff();
}

void loop()
{
}
