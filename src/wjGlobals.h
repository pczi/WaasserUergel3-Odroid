#ifndef wjGlobals_h
#define wjGlobals_h

#include <odroid_go.h>

#define SPECTRUMBAND_COUNT 7
#define LED_COUNT 13
#define WATERJET_COUNT 13

// GENERAL DEFINITIONS
#define topMargin 20
#define singleMargin 2
#define doubleMargin 2 * singleMargin
#define tripleMargin 3 * singleMargin

// VU METER 7 bands + vol + energy
#define Xvum 0
#define Yvum 60
#define Wvum 128
#define Hvum 138

// LIGHT AND WATER ANIMATION 13 leds/waterjets
#define Xlwa Wvum
#define Ylwa Yvum
#define Wlwa (320 - Wvum)
#define Hlwa Hvum

// UPPER INFO BOXES
#define Xupib0 64 * 0
#define Yupib0 0
#define Wupib0 64
#define Hupib0 40
#define Xupib1 64 * 1
#define Yupib1 0
#define Wupib1 64
#define Hupib1 40
#define Xupib2 64 * 2
#define Yupib2 0
#define Wupib2 64
#define Hupib2 40
#define Xupib3 64 * 3
#define Yupib3 0
#define Wupib3 64
#define Hupib3 40
#define Xupib4 64 * 4
#define Yupib4 0
#define Wupib4 64
#define Hupib4 40

// LOWER INFO BOXES
#define Xloib0 64 * 0
#define Yloib0 200
#define Wloib0 64
#define Hloib0 40
#define Xloib1 64 * 1
#define Yloib1 200
#define Wloib1 64
#define Hloib1 40
#define Xloib2 64 * 2
#define Yloib2 200
#define Wloib2 64
#define Hloib2 40
#define Xloib3 64 * 3
#define Yloib3 200
#define Wloib3 64
#define Hloib3 40
#define Xloib4 64 * 4
#define Yloib4 200
#define Wloib4 64
#define Hloib4 40

struct AlaColor
{
  union {
    struct
    {
      uint8_t r;
      uint8_t g;
      uint8_t b;
    };
    uint8_t raw[3];
  };
};

struct SEND_DATA_STRUCTURE
{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  float ampFactor;
  uint8_t waterjetAttack;
  uint8_t waterjetDecay;
  bool EnergyBeatDetected;
  bool BassBeatDetected;
  bool VolumeBeatDetected;
};

struct RECEIVE_DATA_STRUCTURE
{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  float ampFactor;
  uint8_t waterjetAttack;
  uint8_t waterjetDecay;
  // Spectrum analyzer read values.
  byte Spectrum[SPECTRUMBAND_COUNT];
  // average BPM for the last 10 seconds
  int BPM;
  byte EnergyAvg = 0;
  byte EnergyVar = 0;
  byte EnergyStdDev = 0;
  bool EnergyBeatDetected = false;
  bool BassBeatDetected = false;
  bool VolumeBeatDetected = false;
  // Current volume value (sum of all bands / 6)
  byte Volume;
  AlaColor leds[LED_COUNT];
  byte waterjetCurrentHeight[WATERJET_COUNT];
};

class wjGlobals
{
public:
private:
};

#endif