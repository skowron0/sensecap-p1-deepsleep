#include <Arduino.h>
#include <nrf.h>

// SenseCAP P1 - Charger Only Firmware (Optymalizacja prądu uśpienia)
// XIAO nRF52840 + Wio-SX1262

#define LORA_NSS       D4
#define LORA_RESET     D2
#define LORA_BUSY      D3
#define LORA_DIO1      D1
#define LORA_RXEN      D5

static void prepareLowPower()
{
    // 1. Całkowite wyłączenie i odcięcie LoRa (SX1262)
    pinMode(LORA_RXEN, OUTPUT);
    digitalWrite(LORA_RXEN, LOW);

    pinMode(LORA_DIO1, INPUT); // Standardowe wejście bez podciągania

    pinMode(LORA_NSS, OUTPUT);
    digitalWrite(LORA_NSS, HIGH);

    pinMode(LORA_RESET, OUTPUT);
    digitalWrite(LORA_RESET, LOW); // Trzymamy w restarcie

    pinMode(LORA_BUSY, INPUT);

    // 2. Wyłączenie diod LED na płytce XIAO (stan HIGH dla aktywacji niskiej)
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    
    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, HIGH);
    
    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, HIGH);
    
    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, HIGH);

    // 3. Konfiguracja pozostałych pinów w stan bezpieczny
    for (int i = 0; i < PINS_COUNT; i++) {
        if (i == LORA_NSS || i == LORA_RESET || i == LORA_BUSY || i == LORA_DIO1 || i == LORA_RXEN) continue;
        nrf_gpio_cfg_default(i);
    }
}

static void enterSystemOff()
{
    prepareLowPower();

    delay(10);

    // Wejście w tryb nRF52840 SYSTEM OFF
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
    // Usunięto niedostępne USBDevice.detach() – samo przejście w SYSTEM OFF odcina zasilanie USB/PHY
    enterSystemOff();
}

void loop()
{
}
