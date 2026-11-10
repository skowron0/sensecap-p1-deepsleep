#include <Arduino.h>
#include <nrf.h>
#include <nrf_gpio.h>

// =====================================================
// SenseCAP Solar Node P1
// Charger-only firmware
//
// MCU: nRF52840
// Radio: SX1262
// Charger: CN3165
//
// GPS: NOT INSTALLED
// =====================================================


// Arduino pin numbers from SenseCAP Solar variant

#define PIN_LORA_DIO1       1
#define PIN_LORA_RESET      2
#define PIN_LORA_BUSY       3
#define PIN_LORA_NSS        4
#define PIN_LORA_RXEN       5

#define PIN_SPI_SCK         8
#define PIN_SPI_MISO        9
#define PIN_SPI_MOSI        10

#define PIN_LED_WHITE       11
#define PIN_LED_BLUE        12

#define PIN_USER_BUTTON     13

#define PIN_I2C_SDA         14
#define PIN_I2C_SCL         15

#define PIN_BATTERY         16
#define PIN_VBAT_ENABLE     19


// =====================================================
// Disable SX1262 radio
// =====================================================

static void disableRadio()
{
    // Disable external RF receiver switch
    pinMode(PIN_LORA_RXEN, OUTPUT);
    digitalWrite(PIN_LORA_RXEN, LOW);

    // Deselect radio
    pinMode(PIN_LORA_NSS, OUTPUT);
    digitalWrite(PIN_LORA_NSS, HIGH);

    // Hold SX1262 in hardware reset
    pinMode(PIN_LORA_RESET, OUTPUT);
    digitalWrite(PIN_LORA_RESET, LOW);

    // Prevent floating interrupt/status lines
    pinMode(PIN_LORA_DIO1, INPUT_PULLDOWN);
    pinMode(PIN_LORA_BUSY, INPUT_PULLDOWN);

    // SPI inactive
    pinMode(PIN_SPI_SCK, INPUT_PULLDOWN);
    pinMode(PIN_SPI_MOSI, INPUT_PULLDOWN);
    pinMode(PIN_SPI_MISO, INPUT_PULLDOWN);
}


// =====================================================
// Disable LEDs
// =====================================================

static void disableLEDs()
{
    // MeshCore uses LOW to switch these LEDs off.

    pinMode(PIN_LED_WHITE, OUTPUT);
    digitalWrite(PIN_LED_WHITE, LOW);

    pinMode(PIN_LED_BLUE, OUTPUT);
    digitalWrite(PIN_LED_BLUE, LOW);
}


// =====================================================
// Disable battery voltage divider
// =====================================================

static void disableBatteryMeasurement()
{
    // According to SenseCapSolarBoard.cpp:
    // LOW  = battery measurement enabled
    // HIGH = battery measurement disabled

    pinMode(PIN_VBAT_ENABLE, OUTPUT);
    digitalWrite(PIN_VBAT_ENABLE, HIGH);
}


// =====================================================
// Configure unused interfaces
// =====================================================

static void disableUnusedInterfaces()
{
    // I2C is not used.
    // Leave lines as inputs without internal pull resistors.

    pinMode(PIN_I2C_SDA, INPUT);
    pinMode(PIN_I2C_SCL, INPUT);

    // Battery ADC input
    pinMode(PIN_BATTERY, INPUT);

    // GPS is not installed.
    // Do not configure GPS_EN or GPS_STANDBY.
}


// =====================================================
// Enter nRF52840 SYSTEM OFF
// =====================================================

static void enterDeepSleep()
{
    // Enable internal DC/DC converter.
    NRF_POWER->DCDCEN = 1;

    // Clear pending events.
    __SEV();
    __WFE();

    // Enter SYSTEM OFF.
    __DSB();

    NRF_POWER->SYSTEMOFF = 1;

    __DSB();

    // Should never execute.
    while (true)
    {
        __WFE();
    }
}


// =====================================================
// Arduino
// =====================================================

void setup()
{
    // Allow hardware to stabilize.
    delay(20);

    // Disable all unnecessary peripherals.
    disableLEDs();

    disableBatteryMeasurement();

    disableRadio();

    disableUnusedInterfaces();

    // Allow signals to settle.
    delay(20);

    // Sleep forever.
    enterDeepSleep();
}


void loop()
{
    // Never reached.
}
