#include <odroid_go.h>

// bluetooth LE
#include "WiFi.h"
#include "WiFiClientSecure.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BluetoothSerial.h>

// battery voltage reader
#include <driver/adc.h>
#include <esp_adc_cal.h>

// project related
#include "wjGlobals.h"
#include "wjDisplay.h"
#include "wjButton.h"
#include "EasyTransfer.h"

// eeprom
#include "EEPROM.h"

// bluetooth LE
// 0 = don't scan bluetooth devices at startup
// 1 = scan bluetooth devices at startup
#define SCAN_BLUETOOTH_DEVICES 0

// battery voltage reader
#define RESISTANCE_NUM    2
#define DEFAULT_VREF      1100
#define NO_OF_SAMPLES     64
static esp_adc_cal_characteristics_t adc_chars;

double readBatteryVoltage();
void generateRandomRxdata(void);
