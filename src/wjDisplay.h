// methods to draw the display elements
// - info boxes

#ifndef wjDisplay_h
#define wjDisplay_h

#include <odroid_go.h>
#include <wjGlobals.h>
#include <EEPROM.h>

class wjDisplay
{
public:
    wjDisplay(wjGlobals *pgv, RECEIVE_DATA_STRUCTURE *prxdata);
    void clearDisplay();
    void drawVUmeter();
    void drawVUmeterBar(uint16_t Band, uint16_t X, uint16_t Y, uint16_t H, uint16_t W);
    void drawWaterjets();
    void drawWaterjetBar(uint16_t Band, uint16_t X, uint16_t Y, uint16_t H, uint16_t W);

    void drawInfoBox3lines(const char *line1, const char *line2, const char *line3,
                           uint32_t X, uint32_t Y, uint32_t W, uint32_t H, bool negative);
    void drawInfoboxBeatDetect(bool _negative);
    void drawEnergyBeat();
    void drawBassBeat();
    void drawVolBeat();
    void drawInfoboxAmpFactor(bool _negative);
    void drawInfoboxCycleHz(uint16_t cycleHz);
    void drawInfoboxWaterjetAttack(bool _negative);
    void drawInfoboxWaterjetDecay(bool _negative);
    void drawInfoboxBattery(double voltage);
    void drawInfoboxBatteryUptime(double voltage);
    void drawInstructions(void);

private:
    // dependency injection of the global variables
    wjGlobals *_gv;
    RECEIVE_DATA_STRUCTURE *_rxdata;
    byte holdMax[SPECTRUMBAND_COUNT+2];
};

#endif