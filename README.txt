SenseCAP Solar Node P1 - nRF52840 SYSTEM OFF firmware

Purpose:
- Boot and immediately put the nRF52840 into hardware SYSTEM OFF.
- No BLE, LoRa/SX1262 driver, Serial, I2C, SPI, sensor or LED code is started.
- No charger-related GPIO is configured.

Charging:
The CN3165 charging controller is a separate hardware IC. Solar/USB charging
should continue independently of the nRF52840 firmware, as long as the P1
power path and battery are wired normally.

Build:
1. Install PlatformIO Core and the Nordic nRF52 platform.
2. Open this folder in VS Code / PlatformIO.
3. Build environment: seeed_xiao_nrf52840
4. Output UF2 (if the installed platform/board package supports UF2):
   .pio/build/seeed_xiao_nrf52840/firmware.uf2

Flash:
1. Connect the P1 to USB.
2. Double-press RESET to enter the XIAO UF2 bootloader.
3. Copy firmware.uf2 to the mounted XIAO drive.

Important:
SYSTEM OFF is a hardware sleep state, not a complete physical power disconnect.
Any external circuit powered directly from the battery may still draw current.
This firmware intentionally does not drive undocumented P1 GPIOs, because
changing a power-enable or charger-related pin without a verified schematic
could disable charging or affect the second node connected to the battery.
After flashing, verify charging over USB and solar, and measure total battery
current. If the current remains high, it is likely due to external circuitry
(e.g. the SX1262 supply rail or board power path), not CPU execution.
