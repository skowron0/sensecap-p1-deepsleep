#include <Arduino.h>
#include <nrf.h>

// SenseCAP Solar Node P1 - charger-only firmware
// The CN3165 solar charger is a standalone hardware circuit. This firmware
// does not configure charger-related pins; it simply puts the nRF52840 into
// hardware SYSTEM OFF after startup.

static void enterSystemOff()
{
    // Stop the Arduino loop from doing anything else and enter the nRF52840
    // lowest-power state. SYSTEM OFF is exited by a reset-capable wake event
    // (for example, the RESET button or a power cycle).
    __DSB();
    NRF_POWER->SYSTEMOFF = 1;
    __DSB();

    // The CPU should not continue past SYSTEMOFF. Keep a fallback wait loop.
    while (true) {
        __WFE();
    }
}

void setup()
{
    // Do not initialize Serial, BLE, GPIOs, LEDs, SPI, I2C or the SX1262.
    // Leaving board pins at their reset state avoids accidentally changing
    // the external charger/power-control circuitry.
    enterSystemOff();
}

void loop()
{
    // Never reached.
}
