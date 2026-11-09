#include <Arduino.h>
#include <nrf.h>

// SenseCAP Solar Node P1
// Charger-only firmware
// XIAO nRF52840 + Wio-SX1262

// LoRa
#define LORA_NSS       D4
#define LORA_RESET     D2
#define LORA_BUSY      D3
#define LORA_DIO1      D1
#define LORA_RXEN      D5

// LEDs
#define LED_WHITE      D11
#define LED_BLUE       D12

// Battery measurement enable
#define VBAT_ENABLE    D19

static void disablePeripherals()
{
    // ------------------------------------------------
    // 1. Turn off LEDs
    // ------------------------------------------------

    pinMode(LED_WHITE, OUTPUT);
    digitalWrite(LED_WHITE, LOW);

    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, LOW);

    // ------------------------------------------------
    // 2. Disable battery voltage measurement divider
    // ------------------------------------------------

    pinMode(VBAT_ENABLE, OUTPUT);
    digitalWrite(VBAT_ENABLE, HIGH);

    // ------------------------------------------------
    // 3. Disable SX1262 RF switch
    // ------------------------------------------------

    pinMode(LORA_RXEN, OUTPUT);
    digitalWrite(LORA_RXEN, LOW);

    // ------------------------------------------------
    // 4. Put SX1262 into hardware RESET
    // ------------------------------------------------

    pinMode(LORA_NSS, OUTPUT);
    digitalWrite(LORA_NSS, HIGH);

    pinMode(LORA_RESET, OUTPUT);
    digitalWrite(LORA_RESET, LOW);

    // ------------------------------------------------
    // 5. Disable radio interrupt
    // ------------------------------------------------

    pinMode(LORA_DIO1, INPUT);
    pinMode(LORA_BUSY, INPUT);

    // ------------------------------------------------
    // 6. Release SPI pins
    // ------------------------------------------------

    pinMode(D8, INPUT);
    pinMode(D9, INPUT);
    pinMode(D10, INPUT);

    delay(20);
}

static void enterDeepSleep()
{
    // Enable internal DC/DC converter
    NRF_POWER->DCDCEN = 1;

    // Enter SYSTEM OFF
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

    enterDeepSleep();
}

void loop()
{
}
