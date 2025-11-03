#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

// Initialize SD. Prefer using explicit SPI pins for ESP32: cs, sck, miso, mosi.
// Returns true if SD initialized successfully.
bool settings_init(int csPin, int sckPin, int misoPin, int mosiPin);
bool settings_available();
int settings_listFiles(String *outArray, int maxEntries);
bool settings_deleteFile(const char *filename);
bool settings_createSettingsH(const char *filename, const char *contents);
String settings_lastStatus();

#endif // SETTINGS_H
