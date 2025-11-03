#include "settings.h"
#include <SD.h>
#include <SPI.h>

static bool g_available = false;
static String g_lastStatus = "";

// Try to initialize SD using SPI pins (ESP32).
bool settings_init(int csPin, int sckPin, int misoPin, int mosiPin) {
  // On ESP32 try VSPI first with provided pins
  if (sckPin >= 0 && misoPin >= 0 && mosiPin >= 0) {
    SPIClass spiV = SPIClass(VSPI);
    spiV.begin(sckPin, misoPin, mosiPin, csPin);
    g_available = SD.begin(csPin, spiV);
    if (g_available) {
      g_lastStatus = "SD initialized (VSPI, custom pins)";
      return true;
    }
    // try HSPI
    SPIClass spiH = SPIClass(HSPI);
    spiH.begin(sckPin, misoPin, mosiPin, csPin);
    g_available = SD.begin(csPin, spiH);
    if (g_available) {
      g_lastStatus = "SD initialized (HSPI, custom pins)";
      return true;
    }
  }
  // fallback: try default SD.begin(csPin)
  g_available = SD.begin(csPin);
  g_lastStatus = g_available ? "SD initialized (default)" : "SD init failed";
  return g_available;
}

bool settings_available() {
  return g_available;
}

int settings_listFiles(String *outArray, int maxEntries) {
  if (!g_available) return 0;
  int count = 0;
  File root = SD.open("/");
  if (!root) return 0;
  File entry = root.openNextFile();
  while (entry && count < maxEntries) {
    if (!entry.isDirectory()) {
      String name = String(entry.name());
      // strip leading slash if present
      if (name.startsWith("/")) name = name.substring(1);
      outArray[count++] = name;
      Serial.print("SD file: "); Serial.println(name);
    }
    entry = root.openNextFile();
  }
  root.close();
  return count;
}

bool settings_deleteFile(const char *filename) {
  if (!g_available) {
    g_lastStatus = "SD not available";
    return false;
  }
  if (SD.exists(filename)) {
    bool ok = SD.remove(filename);
    g_lastStatus = ok ? String("Deleted: ") + filename : "Delete failed";
    return ok;
  }
  g_lastStatus = "File not found";
  return false;
}

bool settings_createSettingsH(const char *filename, const char *contents) {
  if (!g_available) {
    g_lastStatus = "SD not available";
    return false;
  }
  File f = SD.open(filename, FILE_WRITE);
  if (!f) {
    g_lastStatus = "Create failed";
    return false;
  }
  f.print(contents);
  f.close();
  g_lastStatus = String("Created: ") + filename;
  return true;
}

String settings_lastStatus() {
  return g_lastStatus;
}

// Backward-compatible wrapper: attempts simple SD.begin(csPin)
bool settings_init(int csPin) {
  // try to initialize using default driver (some cores require only cs)
  g_available = SD.begin(csPin);
  g_lastStatus = g_available ? "SD initialized (default)" : "SD init failed";
  return g_available;
}
