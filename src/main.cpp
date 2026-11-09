#include <Arduino.h>
#include <nrf.h>

// SenseCAP P1 - Charger Only Firmware
// XIAO nRF52840 + Wio-SX1262

#define LORA_NSS       D4
#define LORA_RESET     D2
#define LORA_BUSY      D3
#define LORA_DIO1      D1
#define LORA_RXEN      D5

static void disableLoRa()
{
    // Disable RF receiver path
    pinMode(LORA_RXEN, OUTPUT);
    digitalWrite(LORA_RXEN, LOW);

    // Disable radio interrupt
    pinMode(LORA_DIO1, INPUT);

    // Deselect radio SPI
    pinMode(LORA_NSS, OUTPUT);
    digitalWrite(LORA_NSS, HIGH);

    // Hold SX1262 in hardware reset
    pinMode(LORA_RESET, OUTPUT);
    digitalWrite(LORA_RESET, LOW);

    // Wait for radio to enter reset
    delay(20);
}

static void enterSystemOff()
{
    // Enter nRF52840 SYSTEM OFF
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
    disableLoRa();

    enterSystemOff();
}

void loop()
{
}
