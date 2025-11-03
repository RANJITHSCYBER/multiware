#include <Wire.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <SD.h>
#include "settings.h"

// Minimal menu-only sketch

// ===== Pin configuration =====
// ===== Button Pins =====
#define BTN_UP        32
#define BTN_DOWN      33
#define BTN_LEFT      25
#define BTN_RIGHT     26
#define BTN_SELECT    27

// ===== OLED Display (I2C) =====
#define OLED_SDA      21
#define OLED_SCL      22

// ===== MPU6050 (I2C - shared with OLED) =====
#define MPU6050_SDA   21
#define MPU6050_SCL   22

// ===== IR Sensors =====
#define IR_TX         17   // IR Transmitter
#define IR_RX         16   // IR Receiver

// ===== Ultrasonic Sensor =====
#define ULTRASONIC_TRIG  4
#define ULTRASONIC_ECHO  2

// ===== LED =====
#define LED_PIN       5

// ===== SD Card Module (SPI) =====
#define SD_MOSI       23
#define SD_MISO       19
#define SD_SCK        18
#define SD_CS         15

// ===== Motor Driver (L298N / L293D) =====
#define MOTOR_ENA     13
#define MOTOR_IN1     12
#define MOTOR_IN2     14

// ===== Servo Motor =====
#define SERVO_PIN     3

// OLED display setup
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Debounce
const unsigned long debounceMs = 50;
unsigned long lastDebounceTime[5] = {0,0,0,0,0};
bool lastState[5] = {HIGH, HIGH, HIGH, HIGH, HIGH};
bool stableState[5] = {HIGH, HIGH, HIGH, HIGH, HIGH};

// Full menu (example items)
const char* menuItems[] = {
  "1.IR", "2.Ultrasonic", "3.Servo",
  "4.MPU", "5.Robot",      "6.Web",
  "7.RF", "8.Info",        "9.Setting"
};

const int totalItems = sizeof(menuItems) / sizeof(menuItems[0]);

// Layout: 2 columns x 2 rows per page
const int pageCols = 2;
const int pageRows = 2;
const int pageSize = pageCols * pageRows;
const int totalPages = (totalItems + pageSize - 1) / pageSize;

int currentPage = 0;
int localCol = 0;
int localRow = 0;

// Application state
enum AppState {
  STATE_MAIN_MENU,
  STATE_SETTINGS_MENU,
  STATE_SD_MENU,
  STATE_SD_FILE_OPTIONS,
  STATE_WIFI_SETUP,
  STATE_STATUS_MESSAGE
};

AppState appState = STATE_MAIN_MENU;

// Settings submenu
const char* settingsItems[] = { "SD Card Info", "WiFi Setup" };
const int settingsCount = sizeof(settingsItems) / sizeof(settingsItems[0]);
int settingsSelection = 0;

// SD file listing
String sdFiles[64];
int sdFileCount = 0;
int sdListIndex = 0; // currently highlighted file index in listing
bool sdAvailable = false;

// Status message
String statusMsg = "";
// status timing for transient messages
unsigned long statusShownAt = 0;
const unsigned long statusTimeout = 2000;


void setup() {
  Serial.begin(115200);
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tf);

  // initialize I2C with chosen pins
  Wire.begin(OLED_SDA, OLED_SCL);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);

  // initialize SD using explicit SPI pins
  sdAvailable = settings_init(SD_CS, SD_SCK, SD_MISO, SD_MOSI);
  Serial.println(settings_lastStatus());

  drawPage();
}

void loop() {
  readButtonsAndNavigate();
}

void readButtonsAndNavigate() {
  const int pins[5] = {BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_SELECT};
  for (int i = 0; i < 5; ++i) {
    int reading = digitalRead(pins[i]);
    if (reading != lastState[i]) {
      lastDebounceTime[i] = millis();
      lastState[i] = reading;
    }
    if (millis() - lastDebounceTime[i] > debounceMs) {
      if (reading != stableState[i]) {
        stableState[i] = reading;
        if (stableState[i] == LOW) { // pressed
          handleButtonPress(i);
        }
      }
    }
  }
}

void handleButtonPress(int buttonIndex) {
  switch (buttonIndex) {
    case 0: // Up
      if (appState == STATE_MAIN_MENU) {
        if (localRow > 0) localRow--;
        else if (currentPage > 0) { currentPage--; localRow = pageRows - 1; }
      } else if (appState == STATE_SETTINGS_MENU) {
        if (settingsSelection > 0) settingsSelection--;
      } else if (appState == STATE_SD_MENU) {
        if (sdListIndex > 0) sdListIndex--;
      }
      break;
    case 1: // Down
      if (appState == STATE_MAIN_MENU) {
        if (localRow < pageRows - 1) {
          localRow++; if (!localSlotExists(localRow, localCol)) localRow--;
        } else if (currentPage < totalPages - 1) { currentPage++; localRow = 0; }
      } else if (appState == STATE_SETTINGS_MENU) {
        if (settingsSelection < settingsCount - 1) settingsSelection++;
      } else if (appState == STATE_SD_MENU) {
        if (sdListIndex < sdFileCount - 1) sdListIndex++;
      }
      break;
    case 2: // Left
      if (appState == STATE_MAIN_MENU) {
        if (localCol > 0) localCol--; else if (currentPage > 0) { currentPage--; localCol = pageCols - 1; }
      } else if (appState == STATE_SD_FILE_OPTIONS) {
        // Delete file
        if (sdFileCount > 0 && sdListIndex < sdFileCount) {
          const char *fn = sdFiles[sdListIndex].c_str();
          bool ok = settings_deleteFile(fn);
          statusMsg = settings_lastStatus();
          (void)ok;
          sdListFiles();
        }
        appState = STATE_STATUS_MESSAGE;
        statusShownAt = millis();
      }
      break;
    case 3: // Right
      if (appState == STATE_MAIN_MENU) {
        if (localCol < pageCols - 1) { localCol++; if (!localSlotExists(localRow, localCol)) localCol--; }
        else if (currentPage < totalPages - 1) { currentPage++; localCol = 0; }
      } else if (appState == STATE_SD_FILE_OPTIONS) {
        // Create settings.h (simple example)
        if (settings_createSettingsH("settings.h", "// settings.h created from UI\n#define EXAMPLE_SETTING 1\n")) {
          statusMsg = settings_lastStatus();
        } else {
          statusMsg = settings_lastStatus();
        }
        sdListFiles();
        appState = STATE_STATUS_MESSAGE;
        statusShownAt = millis();
      } else if (appState == STATE_SD_MENU) {
        // if SD not available, Right retries initialization
        if (!sdAvailable) {
          sdAvailable = settings_init(SD_CS, SD_SCK, SD_MISO, SD_MOSI);
          statusMsg = settings_lastStatus();
          appState = STATE_STATUS_MESSAGE;
          statusShownAt = millis();
        }
      }
      break;
    case 4: // Select
      {
        if (appState == STATE_MAIN_MENU) {
          int idx = pageIndexToGlobalIndex(currentPage, localRow, localCol);
          if (idx >= 0 && idx < totalItems) {
            Serial.print("Selected: "); Serial.println(menuItems[idx]);
            // If Settings selected (item 9), open settings submenu
            if (strcmp(menuItems[idx], "9.Setting") == 0) {
              appState = STATE_SETTINGS_MENU;
              settingsSelection = 0;
            } else {
              // other menu items: show status message
              statusMsg = String("Opened: ") + menuItems[idx];
              appState = STATE_STATUS_MESSAGE;
              statusShownAt = millis();
            }
          }
        } else if (appState == STATE_SETTINGS_MENU) {
          if (settingsSelection == 0) {
            // SD Card Info
            appState = STATE_SD_MENU;
            sdListFiles();
          } else if (settingsSelection == 1) {
            appState = STATE_WIFI_SETUP;
          }
        } else if (appState == STATE_SD_MENU) {
          if (sdFileCount > 0) {
            appState = STATE_SD_FILE_OPTIONS;
          }
        } else if (appState == STATE_SD_FILE_OPTIONS) {
          // treat select as back to SD list
          appState = STATE_SD_MENU;
        } else if (appState == STATE_WIFI_SETUP) {
          // placeholder: go back
          appState = STATE_SETTINGS_MENU;
        } else if (appState == STATE_STATUS_MESSAGE) {
          // dismiss
          appState = STATE_MAIN_MENU;
        }
      }
      break;
  }
  drawPage();
}

// helper: populate sdFiles[] with filenames from root
void sdListFiles() {
  sdFileCount = settings_listFiles(sdFiles, (int)sizeof(sdFiles)/sizeof(sdFiles[0]));
  sdListIndex = 0;
}

// show a short status message then return to main menu after a timeout

int pageIndexToGlobalIndex(int page, int r, int c) {
  int pageStart = page * pageSize;
  int localIndex = r * pageCols + c;
  int globalIndex = pageStart + localIndex;
  if (globalIndex >= totalItems) return -1;
  return globalIndex;
}

bool localSlotExists(int r, int c) {
  return pageIndexToGlobalIndex(currentPage, r, c) != -1;
}

void drawPage() {
  u8g2.clearBuffer();
  if (appState == STATE_MAIN_MENU) {
    const int boxWidth = 60;
    const int boxHeight = 22;
    const int paddingX = 4;
    const int paddingY = 4;
    const int startX = 2;
    const int startY = 10;
    int pageStart = currentPage * pageSize;
    for (int r = 0; r < pageRows; ++r) {
      for (int c = 0; c < pageCols; ++c) {
        int localIndex = r * pageCols + c;
        int globalIndex = pageStart + localIndex;
        int x = startX + c * (boxWidth + paddingX);
        int y = startY + r * (boxHeight + paddingY);
        if (globalIndex >= totalItems) continue;
        if (r == localRow && c == localCol) {
          u8g2.drawBox(x, y, boxWidth, boxHeight);
          u8g2.setDrawColor(0);
          u8g2.setCursor(x + 6, y + 15);
          u8g2.print(menuItems[globalIndex]);
          u8g2.setDrawColor(1);
        } else {
          u8g2.drawFrame(x, y, boxWidth, boxHeight);
          u8g2.setCursor(x + 6, y + 15);
          u8g2.print(menuItems[globalIndex]);
        }
      }
    }
    char pageBuf[16]; snprintf(pageBuf, sizeof(pageBuf), "Page %d/%d", currentPage + 1, totalPages);
    u8g2.setCursor(2, 8); u8g2.print(pageBuf);
  } else if (appState == STATE_SETTINGS_MENU) {
    u8g2.setCursor(2, 12);
    u8g2.print("Settings:");
    for (int i = 0; i < settingsCount; ++i) {
      int y = 24 + i * 12;
      if (i == settingsSelection) {
        u8g2.drawBox(2, y - 8, 124, 12);
        u8g2.setDrawColor(0);
        u8g2.setCursor(6, y);
        u8g2.print(settingsItems[i]);
        u8g2.setDrawColor(1);
      } else {
        u8g2.setCursor(6, y);
        u8g2.print(settingsItems[i]);
      }
    }
  } else if (appState == STATE_SD_MENU) {
    u8g2.setCursor(2, 12);
    u8g2.print("SD Card Files:");
    if (!sdAvailable) {
      u8g2.setCursor(2, 28); u8g2.print("SD not available");
      u8g2.setCursor(2, 44); u8g2.print("Right=Retry");
    } else if (sdFileCount == 0) {
      u8g2.setCursor(2, 28); u8g2.print("No files");
    } else {
      int start = sdListIndex;
      for (int i = 0; i < 4; ++i) {
        int fi = start + i;
        if (fi >= sdFileCount) break;
        int y = 24 + i * 12;
        if (fi == sdListIndex) {
          u8g2.drawBox(2, y - 8, 124, 12);
          u8g2.setDrawColor(0);
          u8g2.setCursor(6, y);
          u8g2.print(sdFiles[fi]);
          u8g2.setDrawColor(1);
        } else {
          u8g2.setCursor(6, y);
          u8g2.print(sdFiles[fi]);
        }
      }
      u8g2.setCursor(2, 60);
      u8g2.print("Select=Options");
    }
  } else if (appState == STATE_SD_FILE_OPTIONS) {
    u8g2.setCursor(2, 12);
    u8g2.print("File Options:");
    if (sdFileCount > 0 && sdListIndex < sdFileCount) {
      u8g2.setCursor(6, 28);
      u8g2.print(sdFiles[sdListIndex]);
      u8g2.setCursor(2, 44);
      u8g2.print("Left=Delete  Right=Create settings.h");
    }
  } else if (appState == STATE_WIFI_SETUP) {
    u8g2.setCursor(2, 12);
    u8g2.print("WiFi Setup (placeholder)");
    u8g2.setCursor(2, 28);
    u8g2.print("Use serial for config");
  } else if (appState == STATE_STATUS_MESSAGE) {
    u8g2.setCursor(2, 28);
    u8g2.print(statusMsg);
  }
  u8g2.sendBuffer();
  
  if (appState == STATE_STATUS_MESSAGE) {
    if (statusShownAt == 0) statusShownAt = millis();
    if (millis() - statusShownAt > statusTimeout) {
      statusShownAt = 0;
      appState = STATE_MAIN_MENU;
    }
  }
}
