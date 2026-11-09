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

    pinMode(LORA_DIO1, INPUT_NOPULL); // Brak rezystora zapobiega prądom upływu

    pinMode(LORA_NSS, OUTPUT);
    digitalWrite(LORA_NSS, HIGH);

    pinMode(LORA_RESET, OUTPUT);
    digitalWrite(LORA_RESET, LOW); // Trzymamy w restarcie

    pinMode(LORA_BUSY, INPUT_NOPULL);

    // 2. Wyłączenie diod LED na płytce XIAO (jeśli są sterowane programowo)
    // Na XIAO nRF52840 diody LED są zazwyczaj aktywowane stanem LOW (ACTIVE_LOW)
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // Wyłączenie wbudowanej diody
    
    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, HIGH);
    
    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, HIGH);
    
    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, HIGH);

    // 3. Konfiguracja wszystkich pozostałych pinów cyfrowych/analogowych w stan bezpieczny (zapobiega "floating")
    // Piny wiszące w powietrzu generują duże straty prądu. Ustawiamy je jako wejścia z pull-down lub wyjścia LOW.
    // Uwaga: Poniższa pętla iteruje po standardowych pinofigurycjach Arduino na nrf52.
    for (int i = 0; i <PINS_COUNT; i++) {
        // Pomijamy piny LoRa, które już skonfigurowaliśmy
        if (i == LORA_NSS || i == LORA_RESET || i == LORA_BUSY || i == LORA_DIO1 || i == LORA_RXEN) continue;
        
        // Konfiguracja niewykorzystanych pinów jako wejść z podciągnięciem do masy (PullDown)
        // Zapobiega to oscylacjom i prądom błądzącym
        nrf_gpio_cfg_default(i); // Reset do stanu domyślnego
    }
}

static void enterSystemOff()
{
    prepareLowPower();

    // Odczekaj chwilę, aby stany stabilizatorów i kondensatorów się ustabilizowały
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
    // Wyłączamy Serial USB, aby nie pobierał prądu przez konwerter
    Serial.end();
    USBDevice.detach();

    enterSystemOff();
}

void loop()
{
}
