#include <Arduino.h>
#include <nrf.h>

// =====================================================
// SenseCAP Solar Node P1
// Charger-only firmware
// XIAO nRF52840 + SX1262 + P25Q16H
// =====================================================

// SX1262
#define LORA_NSS       4
#define LORA_RESET     2
#define LORA_BUSY      3
#define LORA_DIO1      1
#define LORA_RXEN      5

// LEDs
#define LED_WHITE      11
#define LED_BLUE       12

// Battery divider
#define VBAT_ENABLE    19

// GPS (not installed)
#define GPS_EN         18

// External QSPI Flash P25Q16H
#define FLASH_SCK      21
#define FLASH_CS       22
#define FLASH_IO0      23
#define FLASH_IO1      24
#define FLASH_IO2      25
#define FLASH_IO3      26


// =====================================================
// Send command to external SPI flash
// =====================================================

static void flashSendByte(uint8_t value)
{
    for (int i = 7; i >= 0; i--)
    {
        digitalWrite(FLASH_IO0, (value >> i) & 1);

        delayMicroseconds(2);

        digitalWrite(FLASH_SCK, HIGH);

        delayMicroseconds(2);

        digitalWrite(FLASH_SCK, LOW);
    }
}


// =====================================================
// Put P25Q16H into Deep Power Down
// Command B9h
// =====================================================

static void disableExternalFlash()
{
    pinMode(FLASH_CS, OUTPUT);
    digitalWrite(FLASH_CS, HIGH);

    pinMode(FLASH_SCK, OUTPUT);
    digitalWrite(FLASH_SCK, LOW);

    pinMode(FLASH_IO0, OUTPUT);
    digitalWrite(FLASH_IO0, LOW);

    delayMicroseconds(10);

    // Select flash
    digitalWrite(FLASH_CS, LOW);

    // Deep Power Down command
    flashSendByte(0xB9);

    // Deselect flash
    digitalWrite(FLASH_CS, HIGH);

    delayMicroseconds(100);

    // Keep QSPI lines in low-power states
    digitalWrite(FLASH_SCK, LOW);
    digitalWrite(FLASH_IO0, LOW);

    pinMode(FLASH_IO0, INPUT_PULLDOWN);
    pinMode(FLASH_IO1, INPUT_PULLDOWN);
    pinMode(FLASH_IO2, INPUT_PULLDOWN);
    pinMode(FLASH_IO3, INPUT_PULLDOWN);
}


// =====================================================
// Disable SX1262
// =====================================================

static void disableLoRa()
{
    // Disable RF receiver switch
    pinMode(LORA_RXEN, OUTPUT);
    digitalWrite(LORA_RXEN, LOW);

    // Deselect radio
    pinMode(LORA_NSS, OUTPUT);
    digitalWrite(LORA_NSS, HIGH);

    // Hardware reset
    pinMode(LORA_RESET, OUTPUT);
    digitalWrite(LORA_RESET, LOW);

    // Disable interrupts
    pinMode(LORA_DIO1, INPUT_PULLDOWN);
    pinMode(LORA_BUSY, INPUT_PULLDOWN);

    // SPI
    pinMode(8, OUTPUT);
    digitalWrite(8, LOW);

    pinMode(9, INPUT_PULLDOWN);

    pinMode(10, OUTPUT);
    digitalWrite(10, LOW);
}


// =====================================================
// Disable all unnecessary peripherals
// =====================================================

static void disablePeripherals()
{
    // LEDs OFF
    pinMode(LED_WHITE, OUTPUT);
    digitalWrite(LED_WHITE, LOW);

    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, LOW);

    // Disable battery voltage divider
    pinMode(VBAT_ENABLE, OUTPUT);
    digitalWrite(VBAT_ENABLE, HIGH);

    // GPS not installed - keep its supply disabled
    pinMode(GPS_EN, OUTPUT);
    digitalWrite(GPS_EN, LOW);

    // Disable LoRa
    disableLoRa();

    // Disable external QSPI flash
    disableExternalFlash();

    // Disable unused Grove I2C pins
    pinMode(14, INPUT_PULLDOWN);
    pinMode(15, INPUT_PULLDOWN);

    delay(50);
}


// =====================================================
// nRF52840 SYSTEM OFF
// =====================================================

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
