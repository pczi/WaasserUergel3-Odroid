#include "main.h"

// see this for ESP32 server / scanner / client
// https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
// youtube HC-05: Arduino Two-Way Bluetooth Communication Tutorial

// https://magazine.odroid.com/article/odroid-go-game-kit-a-handheld-gaming-console-to-celebrate-hardkernels-10th-anniversary/
// https://wiki.odroid.com/odroid_go/arduino/01_arduino_setup
// https://wiki.odroid.com/odroid_go/arduino/02_hello_world

// OSX: install Driver USB to UART BRIDGE VCP
// https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers
// madsci1016/Arduino-EasyTransfer

// define the prototypes here, as private in main.h does not seem to work
// void serialBTwriteString(String stringData);

uint8_t line = 0;

int scanTime = 5; //In seconds

//create objects
EasyTransfer ETin, ETout;
//give a name to the group of data
RECEIVE_DATA_STRUCTURE rxdata;
SEND_DATA_STRUCTURE txdata;

BLEScan *pBLEScan;
BluetoothSerial SerialBT;
wjGlobals gv;
wjDisplay wjD(&gv, &rxdata);
wjButton wjB(&gv, &wjD, &txdata);

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
  }
};

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);

  GO.begin();
  wjB.showInstructions();
  wjD.clearDisplay();

  // bluetooth LE
  //BLEDevice::init("");
  //pBLEScan = BLEDevice::getScan(); //create new scan
  //pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  //pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  //pBLEScan->setInterval(100);
  //pBLEScan->setWindow(99); // less or equal setInterval value

#if SCAN_BLUETOOTH_DEVICES == 1
  Serial.println("Scanning...");
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  delay(500);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
#endif

  // battery voltage reader
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);

  // project related
  txdata.ampFactor = 1.4;
  wjD.drawInfoboxAmpFactor(false);
  txdata.waterjetAttack = 40;
  wjD.drawInfoboxWaterjetAttack(false);
  txdata.waterjetDecay = 20;
  wjD.drawInfoboxWaterjetDecay(false);

  // start serial communication with master on port
  //  Serial1.begin(9600);
  //start the library, pass in the data details and the name of the serial port.
  ETin.begin(details(rxdata), &SerialBT);
  ETout.begin(details(txdata), &SerialBT);

}

void loop()
{
  // put your main code here, to run repeatedly:
  GO.update();

  /*  static uint16_t btCount = 0;
  static uint32_t btNextMillisWrite = 0;
  if (millis() > btNextMillisWrite ) {
    char s[10]; 
    ltoa(btCount++, s, DEC);
    Serial.print("bt_test_");
    Serial.println(s);
    serialBTwriteString("bt_test_");
    serialBTwriteString(s);
    btNextMillisWrite = millis() + 1000;
  }
*/

  // process buttons and redraw info boxes if needed
  bool upPressed = wjB.upWasPressed();
  bool downPressed = wjB.downWasPressed();
  wjB.ampFactorChange(upPressed, downPressed);
  wjB.waterjetAttackChange(upPressed, downPressed);
  wjB.waterjetDecayChange(upPressed, downPressed);
  wjB.bassBeatChange();

  //then we will go ahead and send that data out
  ETout.sendData();
  // reset the beats
  txdata.EnergyBeatDetected = false;
  txdata.BassBeatDetected = false;
  txdata.VolumeBeatDetected = false;

  //check and see if a data packet has come in.
  //if (ETin.receiveData())
  //{
  //}

  // static bool pl = true;
  // if (GO.BtnA.isPressed() == 1)
  // {
  //   GO.lcd.fillRect(0, 0, 320, 20, BLACK);
  //   GO.lcd.setTextSize(2);
  //   GO.lcd.setCursor(0, 0);
  //   GO.Speaker.beep();
  //   if (pl)
  //   {
  //     GO.lcd.setTextColor(GREEN);
  //     GO.lcd.print("Hello, PIT !");
  //   }
  //   else
  //   {
  //     GO.lcd.setTextColor(RED);
  //     GO.lcd.print("Hello, LOLLY ;-)");
  //   }
  //   pl = !pl;
  //   while (GO.BtnA.isPressed() == 1)
  //   {
  //   GO.update();
  //     delay(100);
  //   }
  // }
  // GO.Speaker.beep();

  static uint32_t nextRefreshAnimations = millis();
  if (millis() > nextRefreshAnimations)
  {
    // generate dummy data to test display drawing
    generateRandomRxdata();
    wjD.drawVUmeter();
    wjD.drawWaterjets();
    wjD.drawEnergyBeat();
    wjD.drawBassBeat();
    wjD.drawVolBeat();
    // refresh @ 25 hz
    nextRefreshAnimations = millis() + 40;
  }

  static uint32_t nextRefreshCycleHz = millis();
  static uint16_t cycleHz = 0;
  cycleHz++;
  if (millis() > nextRefreshCycleHz)
  {
    wjD.drawInfoboxCycleHz(cycleHz);
    double voltage = readBatteryVoltage();
    wjD.drawInfoboxBattery(voltage);
    wjD.drawInfoboxBatteryUptime(voltage);
    cycleHz = 0;
    // refresh @ 1 hz
    nextRefreshCycleHz = millis() + 1000;
  }
}

/*
void serialBTwriteString(String stringData) 
{
  for (int i = 0; i < stringData.length(); i++)
  {
    serialBT.write(stringData[i]);
  }
}
 */

static BLERemoteCharacteristic *pRemoteCharacteristic;
//Service UUID of fitnessband obtained through nRF connect application
static BLEUUID serviceUUID("0000fee7-0000-1000-8000-00805f9b34fb");
//Characteristic  UUID of fitnessband obtained through nRF connect application
static BLEUUID charUUID("0000fee7-0000-1000-8000-00805f9b34fb");
//Hardware Bluetooth MAC of my fitnessband, will vary for every band obtained through nRF connect application
String My_BLE_Address = "c7:f0:69:f0:68:81";

bool connectToserver(BLEAddress pAddress)
{
  BLEClient *pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  // Connect to the BLE Server.
  pClient->connect(pAddress);
  Serial.println(" - Connected to fitnessband");
  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService != nullptr)
  {
    Serial.println(" - Found our service");
    return true;
  }
  else
    return false;

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic != nullptr)
    Serial.println(" - Found our characteristic");
  return true;
}

double readBatteryVoltage()
{
  uint32_t adc_reading = 0;
  for (int i = 0; i < NO_OF_SAMPLES; i++)
  {
    adc_reading += adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_0);
  }
  adc_reading /= NO_OF_SAMPLES;

  return (double)esp_adc_cal_raw_to_voltage(adc_reading, &adc_chars) * RESISTANCE_NUM / 1000;
}

void generateRandomRxdata(void)
{
  // generate random data to test
  rxdata.ampFactor = txdata.ampFactor;
  rxdata.waterjetAttack = txdata.waterjetAttack;
  rxdata.waterjetDecay = txdata.waterjetDecay;
  // Spectrum analyzer read values.
  uint32_t sumvol = 0;
  for (uint16_t i = 0; i < SPECTRUMBAND_COUNT; i++)
  {
    rxdata.Spectrum[i] += (i + 1);
    sumvol += rxdata.Spectrum[i];
  }
  // Current volume value (sum of all bands / 6)
  rxdata.Volume = sumvol / SPECTRUMBAND_COUNT;
  // average BPM for the last 10 seconds
  rxdata.BPM += 5;
  rxdata.EnergyAvg += 2;
  rxdata.EnergyVar += 3;
  rxdata.EnergyStdDev += 1;
  // Energybeat every 800 ms
  static uint32_t EnergyBeatMillis = 0;
  if (millis() > EnergyBeatMillis)
  {
    rxdata.EnergyBeatDetected = true;
    EnergyBeatMillis = millis() + 800;
  }
  else
  {
    rxdata.EnergyBeatDetected = false;
  }
  // Bassbeat every 450 ms
  static uint32_t BassBeatMillis = 0;
  if (millis() > BassBeatMillis)
  {
    rxdata.BassBeatDetected = true;
    BassBeatMillis = millis() + 450;
  }
  else
  {
    rxdata.BassBeatDetected = false;
  }
  // Volumebeat every 230 ms
  static uint32_t VolBeatMillis = 0;
  if (millis() > VolBeatMillis)
  {
    rxdata.VolumeBeatDetected = true;
    VolBeatMillis = millis() + 230;
  }
  else
  {
    rxdata.VolumeBeatDetected = false;
  }
  // Current led rgb values
  for (uint16_t i = 0; i < LED_COUNT; i++)
  {
    rxdata.leds[i].r += (i + 1) * 1;
    rxdata.leds[i].g += (i + 1) * 2;
    rxdata.leds[i].b += (i + 1) * 3;
  }
  // Current waterjet height
  for (uint16_t i = 0; i < WATERJET_COUNT; i++)
  {
    rxdata.waterjetCurrentHeight[i] += (i + 1);
  }
}