// methods to draw the display elements
// - info boxes

#include "wjDisplay.h"

wjDisplay::wjDisplay(wjGlobals *pgv, RECEIVE_DATA_STRUCTURE *prxdata)
{
    _gv = pgv;
    _rxdata = prxdata;
}

/*********** clear the matrix ***********************/

void wjDisplay::clearDisplay()
{
    GO.lcd.clearDisplay();
    GO.lcd.setTextSize(2);
    GO.lcd.setTextColor(RED);
    GO.lcd.drawCentreString("Biissener Waasseruergel", (320 / 2), 42, 1);
}

/*********** draw the VU meter on the matrix ***********************/

void wjDisplay::drawVUmeter()
{
    GO.lcd.drawRoundRect(Xvum + singleMargin, Yvum + singleMargin, Wvum - doubleMargin, Hvum - doubleMargin, 3, BLUE);
    GO.lcd.setTextSize(1);
    GO.lcd.setTextColor(BLUE);
    GO.lcd.drawCentreString("VUmeter-Vol-Energy", Xvum + (Wvum / 2), Yvum + 6, 1);

    static uint32_t holdMaxNextDecay;
    const byte holdMaxDecay = 1;
    const uint32_t holdMaxMillisCycle = 50;
    // adjust the holdmax if needed
    if (millis() > holdMaxNextDecay)
    {
        for (int i = 0; i < (SPECTRUMBAND_COUNT + 2); i++)
        {
            holdMax[i] = max(0, holdMax[i] - holdMaxDecay);
        }
        holdMaxNextDecay = millis() + holdMaxMillisCycle;
    }

    // draw spectrum as vumeter
    for (int i = 0; i < SPECTRUMBAND_COUNT; i++)
    {
        // draw 7 spectrum bands, keeps space for 2 more bands (volume + energy)
        this->drawVUmeterBar(i, Xvum + tripleMargin, Yvum + topMargin, Hvum - topMargin - tripleMargin, (uint16_t)(Wvum - tripleMargin) / (SPECTRUMBAND_COUNT + 2));
    }
    // draw volume
    this->drawVUmeterBar((SPECTRUMBAND_COUNT), Xvum + tripleMargin, Yvum + topMargin, Hvum - topMargin - tripleMargin, (uint16_t)(Wvum - tripleMargin) / (SPECTRUMBAND_COUNT + 2));
    // draw energy variance
    this->drawVUmeterBar((SPECTRUMBAND_COUNT + 1), Xvum + tripleMargin, Yvum + topMargin, Hvum - topMargin - tripleMargin, (uint16_t)(Wvum - tripleMargin) / (SPECTRUMBAND_COUNT + 2));
}

void wjDisplay::drawVUmeterBar(uint16_t Band, uint16_t X, uint16_t Y, uint16_t H, uint16_t W)
{
    byte adjBandLevel;
    switch (Band)
    {
    case (SPECTRUMBAND_COUNT):
        // volume
        adjBandLevel = map(_rxdata->Volume, 0, 255, 0, H);
        if (_rxdata->Volume > holdMax[Band])
        {
            holdMax[Band] = _rxdata->Volume;
        }
        break;
    case (SPECTRUMBAND_COUNT + 1):
        // energy variance
        adjBandLevel = map(_rxdata->EnergyVar, 0, 255, 0, H);
        if (_rxdata->EnergyVar > holdMax[Band])
        {
            holdMax[Band] = _rxdata->EnergyVar;
        }
        break;
    default:
        adjBandLevel = map(_rxdata->Spectrum[Band], 0, 255, 0, H);
        if (_rxdata->Spectrum[Band] > holdMax[Band])
        {
            holdMax[Band] = _rxdata->Spectrum[Band];
        }
        break;
    }
    uint32_t adjX = X + (Band * W);
    uint32_t adjW = W - 2;
    uint32_t Y0 = Y + H;
    uint32_t Y1GrOr = Y + (H * 0.50);
    uint32_t Y2OrRe = Y + (H * 0.25);
    uint32_t Y3 = Y;
    uint32_t YBandLevel = Y0 - adjBandLevel;
    uint32_t Ytmp;

    // ----------------------
    // draw the band
    // ----------------------

    // draw green part
    Ytmp = max(YBandLevel, Y1GrOr - 1);
    GO.lcd.fillRect(adjX, Ytmp, adjW, Y0 - Ytmp, GREEN);

    // draw orange part
    Ytmp = max(YBandLevel, Y2OrRe - 1);
    if (Ytmp < Y1GrOr)
    {
        GO.lcd.fillRect(adjX, Ytmp, adjW, Y1GrOr - Ytmp, ORANGE);
    }

    // draw red part
    Ytmp = max(YBandLevel, Y3 - 1);
    if (Ytmp < Y2OrRe)
    {
        GO.lcd.fillRect(adjX, Ytmp, adjW, Y2OrRe - Ytmp, RED);
    }

    // ----------------------
    // draw the holdMaxMarker
    // ----------------------

    byte YholdMax = Y0 - max(adjW, map(holdMax[Band], 0, 255, 0, H));

    // draw the black part between red and holdMax only if
    if (Ytmp > YholdMax)
    {
        GO.lcd.fillRect(adjX, YholdMax + W, adjW, Ytmp - YholdMax - W, BLACK);
    }
    // draw a white square
    GO.lcd.fillRect(adjX, YholdMax, adjW, adjW, WHITE);
    // draw the black part above holdMax
    GO.lcd.fillRect(adjX, Y3, adjW, YholdMax - Y3, BLACK);
}

/*********** draw the Waterjets on the matrix ***********************/

void wjDisplay::drawWaterjets()
{
    GO.lcd.drawRoundRect(Xlwa + singleMargin, Ylwa + singleMargin, Wlwa - doubleMargin, Hlwa - doubleMargin, 3, BLUE);
    GO.lcd.setTextSize(1);
    GO.lcd.setTextColor(BLUE);
    GO.lcd.drawCentreString("light & water animation", Xlwa + (Wlwa / 2), Ylwa + 6, 1);

    // draw lights as bars
    for (uint16_t i = 0; i < WATERJET_COUNT; i++)
    {
        this->drawWaterjetBar(i, Xlwa + tripleMargin, Ylwa + topMargin, Hlwa - topMargin - tripleMargin, (uint16_t)(Wlwa - tripleMargin) / WATERJET_COUNT);
    }
}

void wjDisplay::drawWaterjetBar(uint16_t Band, uint16_t X, uint16_t Y, uint16_t H, uint16_t W)
{
    byte adjBandLevel = max(W, map(_rxdata->waterjetCurrentHeight[Band], 0, 255, 0, H));
    uint32_t adjX = X + (Band * W);
    uint32_t adjW = W - 2;
    const uint32_t Y0 = Y + H;
    const uint32_t Y3 = Y;
    uint32_t Ytmp = Y0 - adjBandLevel;
    uint16_t PixelColor = GO.lcd.color565(_rxdata->leds[Band].r, _rxdata->leds[Band].g, _rxdata->leds[Band].b);

    // ----------------------
    // draw the band
    // ----------------------

    GO.lcd.fillRect(adjX, Ytmp + adjW, adjW, Y0 - Ytmp - adjW, PixelColor);
    GO.lcd.fillRect(adjX, Ytmp, adjW, adjW, WHITE);
    GO.lcd.fillRect(adjX, Y3, adjW, Ytmp - Y3, PixelColor);
}

void wjDisplay::drawInfoboxBeatDetect(bool _negative)
{
    drawInfoBox3lines("En-Bas-Vol", "beat (A)", "", Xloib0, Yloib0, Wloib0, Hloib0, _negative);
    drawEnergyBeat();
    drawBassBeat();
    drawVolBeat();
}

void wjDisplay::drawEnergyBeat()
{
    const int period = 10;
    static unsigned long time_start = 0;
    static bool status_on = false;
    const uint32_t Wrect = 15;
    const uint32_t Hrect = 12;

    if (_rxdata->EnergyBeatDetected)
    {
        // start cycle
        time_start = millis();
        status_on = true;
    }

    if (millis() > time_start + period)
    {
        status_on = false;
    }

    if (status_on)
    {
        GO.lcd.fillRoundRect(Xloib0 + ((Wloib0 - 3 * Wrect) / 2) + (Wrect * 0), Yloib0 + 22, Wrect, Hrect, 3, RED);
    }
    else
    {
        GO.lcd.fillRoundRect(Xloib0 + ((Wloib0 - 3 * Wrect) / 2) + (Wrect * 0), Yloib0 + 22, Wrect, Hrect, 3, BLACK);
    }
}

void wjDisplay::drawBassBeat()
{
    const int period = 10;
    static unsigned long time_start = 0;
    static bool status_on = false;
    const uint32_t Wrect = 15;
    const uint32_t Hrect = 12;

    if (_rxdata->BassBeatDetected)
    {
        // start cycle
        time_start = millis();
        status_on = true;
    }

    if (millis() > time_start + period)
    {
        status_on = false;
    }

    if (status_on)
    {
        GO.lcd.fillRoundRect(Xloib0 + ((Wloib0 - 3 * Wrect) / 2) + (Wrect * 1), Yloib0 + 22, Wrect, Hrect, 3, RED);
    }
    else
    {
        GO.lcd.fillRoundRect(Xloib0 + ((Wloib0 - 3 * Wrect) / 2) + (Wrect * 1), Yloib0 + 22, Wrect, Hrect, 3, BLACK);
    }
}

void wjDisplay::drawVolBeat()
{
    const int period = 10;
    static unsigned long time_start = 0;
    static bool status_on = false;
    const uint32_t Wrect = 15;
    const uint32_t Hrect = 12;

    if (_rxdata->VolumeBeatDetected)
    {
        // start cycle
        time_start = millis();
        status_on = true;
    }

    if (millis() > time_start + period)
    {
        status_on = false;
    }

    if (status_on)
    {
        GO.lcd.fillRoundRect(Xloib0 + ((Wloib0 - 3 * Wrect) / 2) + (Wrect * 2), Yloib0 + 22, Wrect, Hrect, 3, RED);
    }
    else
    {
        GO.lcd.fillRoundRect(Xloib0 + ((Wloib0 - 3 * Wrect) / 2) + (Wrect * 2), Yloib0 + 22, Wrect, Hrect, 3, BLACK);
    }
}

void wjDisplay::drawInfoboxAmpFactor(bool _negative)
{
    char line3[6] = {0, 0, 0, 0, 0, 0};
    dtostrf(_rxdata->ampFactor, 3, 1, line3);
    drawInfoBox3lines("Amplify", "factor", line3, Xloib1, Yloib1, Wloib1, Hloib1, _negative);
}

void wjDisplay::drawInfoboxCycleHz(uint16_t cycleHz)
{
    char line3[6] = {0, 0, 0, 0, 0, 0};
    itoa(cycleHz, line3, DEC);
    drawInfoBox3lines("Cycle", "Hz", line3, Xloib2, Yloib2, Wloib2, Hloib2, false);
}

void wjDisplay::drawInfoboxWaterjetAttack(bool _negative)
{
    char line3[6] = {0, 0, 0, 0, 0, 0};
    itoa(_rxdata->waterjetAttack, line3, DEC);
    drawInfoBox3lines("Waterjet", "attack", line3, Xloib3, Yloib3, Wloib3, Hloib3, _negative);
}

void wjDisplay::drawInfoboxWaterjetDecay(bool negative)
{
    char line3[6] = {0, 0, 0, 0, 0, 0};
    itoa(_rxdata->waterjetDecay, line3, DEC);
    drawInfoBox3lines("Waterjet", "decay", line3, Xloib4, Yloib4, Wloib4, Hloib4, negative);
}

void wjDisplay::drawInfoboxBatteryUptime(double voltage)
{
    // run this only every second
    static uint32_t next_time = millis();
    if (millis() < next_time)
    {
        return;
    }
    next_time = millis() + 1000;

    static uint32_t UptimeMinutes = EEPROM.readUInt(0);
    static double oldVoltage = voltage;
    static bool loading = false;
    static bool refresh = true;
    static uint32_t millisSinceLastUptimeUpdate = 0;
    // check if we are charging or uncharging
    if (voltage > oldVoltage + 0.01)
    {
        if (!loading)
        {
            refresh = true;
        }
        loading = true;
        // we are loading, set BatteryUptime to 0
        if (UptimeMinutes != 0 && voltage > 4.1)
        {
            refresh = true;
            UptimeMinutes = 0;
            EEPROM.writeUInt(0, UptimeMinutes);
        }
    }
    if (voltage < oldVoltage - 0.01)
    {
        if (loading)
        {
            refresh = true;
        }
        loading = false;
        // we are running on battery, increment every minute
        if (millis() > millisSinceLastUptimeUpdate + 60000)
        {
            refresh = true;
            UptimeMinutes += 1;
            EEPROM.writeUInt(0, UptimeMinutes);
            millisSinceLastUptimeUpdate = millis();
        }
    }

    if (refresh)
    {
        char line3[6] = {0, 0, 0, 0, 0, 0};
        itoa(UptimeMinutes, line3, DEC);
        drawInfoBox3lines("on Batt", "minutes", line3, Xupib3, Yupib3, Wupib3, Hupib3, false);
        oldVoltage = voltage;
        refresh = false;
    }
}

void wjDisplay::drawInfoboxBattery(double voltage)
{
    // run this only every second
    static uint32_t next_time = millis();
    if (millis() < next_time)
    {
        return;
    }
    next_time = millis() + 1000;

    static double oldVoltage = voltage;
    static bool loading = false;
    // check if we are charging or uncharging
    if (voltage > oldVoltage)
    {
        loading = true;
    }
    if (voltage < oldVoltage)
    {
        loading = false;
    }

    static uint8_t oldPercentage = 0;
    static unsigned long time_next = 0;
    static bool status_on = false;

    char line3[6] = {0, 0, 0, 0, 0, 0};
    // usually we have values between 88 and 113 %
    uint8_t percentage = constrain(voltage / 3.7 * 100, 90, 110);
    percentage = map(percentage, 90, 110, 0, 100);
    if (percentage != oldPercentage ||
        (percentage < 110 && millis() > time_next))
    {
        itoa(percentage, line3, DEC);
        for (uint16_t i = 0; i < 6; i++)
        {
            if (line3[i] == 0)
            {
                line3[i] = '%';
                break;
            }
        }
        if (percentage > 25)
        {
            // fixed display
            drawInfoBox3lines("Battery", "", line3, Xupib4, Yupib4, Wupib4, Hupib4, loading);
        }
        else
        {
            // blinking display
            if (millis() > time_next)
            {
                status_on = !status_on;
                if (!status_on)
                {
                    line3[0] = 0;
                }
                drawInfoBox3lines("Battery", "", line3, Xupib4, Yupib4, Wupib4, Hupib4, false);
                time_next = millis() + (20 * percentage);
            }
        }
    }
}

void wjDisplay::drawInfoBox3lines(const char *line1, const char *line2, const char *line3,
                                  uint32_t X, uint32_t Y, uint32_t W, uint32_t H, bool negative)
{
    GO.lcd.fillRect(X, Y, W, H, BLACK);
    if (negative)
    {
        GO.lcd.fillRoundRect(X + singleMargin, Y, W - doubleMargin, H - doubleMargin, 3, BLUE);
        GO.lcd.setTextColor(BLACK);
    }
    else
    {
        GO.lcd.drawRoundRect(X + singleMargin, Y, W - doubleMargin, H - doubleMargin, 3, BLUE);
        GO.lcd.setTextColor(BLUE);
    }
    GO.lcd.setTextSize(1);
    GO.lcd.drawCentreString(line1, X + (W / 2), Y + 3, 1);
    GO.lcd.drawCentreString(line2, X + (W / 2), Y + 10, 1);
    GO.lcd.setTextSize(2);
    GO.lcd.setTextColor(GREEN);
    GO.lcd.drawCentreString(line3, X + (W / 2), Y + 20, 1);
}

void wjDisplay::drawInstructions(void)
{
    uint32_t line = 50;
    uint32_t linespacing = 18;
    uint32_t tab = 110;
    GO.lcd.clearDisplay();
    GO.lcd.setTextSize(2);
    GO.lcd.setTextColor(RED);
    GO.lcd.drawCentreString("Biissener Waasseruergel", (320 / 2), 22, 1);
    GO.lcd.setTextSize(1);
    GO.lcd.setTextFont(2);
    GO.lcd.setTextColor(GREEN);

    GO.lcd.setCursor(0, line);
    GO.lcd.print("(A)");
    GO.lcd.setCursor(tab, line);
    GO.lcd.print("force to send beat ");
    line += linespacing;

    GO.lcd.setCursor(0, line);
    GO.lcd.print("(Speakr+Up/Dwn)");
    GO.lcd.setCursor(tab, line);
    GO.lcd.print("incr / decr signal amplification ");
    line += linespacing;

    GO.lcd.setCursor(0, line);
    GO.lcd.print("(Select+Up/Dwn)");
    GO.lcd.setCursor(tab, line);
    GO.lcd.print("incr / decr waterjet attack ");
    line += linespacing;

    GO.lcd.setCursor(0, line);
    GO.lcd.print("(Start +Up/Dwn)");
    GO.lcd.setCursor(tab, line);
    GO.lcd.print("incr / decr waterjet decay ");
    line += linespacing;

    GO.lcd.setTextSize(1);
    GO.lcd.setTextColor(BLUE);
    GO.lcd.drawCentreString("(c) 2019 NoLimits / Peter Czibula", (320 / 2), 190, 2);
    GO.lcd.setTextSize(2);
    GO.lcd.setTextColor(RED);
    GO.lcd.drawCentreString("press (A) to start", (320 / 2), 210, 1);
}