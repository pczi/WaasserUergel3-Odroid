// methods to manage all the buttons
// - menu + volume + select + start
// - up + down

#include "wjButton.h"

wjButton::wjButton(wjGlobals *pgv, wjDisplay *pwjD, SEND_DATA_STRUCTURE *ptxdata)
{
    _gv = pgv;
    _wjD = pwjD;
    _txdata = ptxdata;
}

/*
    force BassBeat if button A is pressed
*/

void wjButton::bassBeatChange()
{
    static bool btnPressed = true;
    if (GO.BtnA.isPressed() != 0 && !btnPressed)
    {
        // someone pressed the A button
        btnPressed = true;
        _wjD->drawInfoboxBeatDetect(btnPressed);
        _txdata->EnergyBeatDetected = true;
        _txdata->BassBeatDetected = true;
        _txdata->VolumeBeatDetected = true;
    }
    if (GO.BtnA.isReleased() != 0 && btnPressed)
    {
        // someone released the A button
        btnPressed = false;
        _wjD->drawInfoboxBeatDetect(btnPressed);
    }
}

/*
    incr/decr ampFactor if buttons are pressed
    and redraw infobox if needed
*/

void wjButton::ampFactorChange(bool upPressed, bool downPressed)
{
    static bool btnPressed = true;
    if (GO.BtnVolume.isPressed() != 0 && !btnPressed)
    {
        // someone pressed the volume button
        btnPressed = true;
        _wjD->drawInfoboxAmpFactor(btnPressed);
    }
    if (GO.BtnVolume.isReleased() != 0 && btnPressed)
    {
        // someone released the volume button
        btnPressed = false;
        _wjD->drawInfoboxAmpFactor(btnPressed);
    }

    if (btnPressed && upPressed)
    {
        if (_txdata->ampFactor < 5)
            _txdata->ampFactor = _txdata->ampFactor + .1;
        _wjD->drawInfoboxAmpFactor(btnPressed);
    }
    if (btnPressed && downPressed)
    {
        if (_txdata->ampFactor > 0)
            _txdata->ampFactor = _txdata->ampFactor - .1;
        _wjD->drawInfoboxAmpFactor(btnPressed);
    }
}

/*
    incr/decr waterjetAttack if buttons are pressed
    and redraw infobox if needed
*/

void wjButton::waterjetAttackChange(bool upPressed, bool downPressed)
{
    static bool btnPressed = true;
    if (GO.BtnSelect.isPressed() != 0 && !btnPressed)
    {
        // someone pressed the volume button
        btnPressed = true;
        _wjD->drawInfoboxWaterjetAttack(btnPressed);
    }
    if (GO.BtnSelect.isReleased() != 0 && btnPressed)
    {
        // someone released the volume button
        btnPressed = false;
        _wjD->drawInfoboxWaterjetAttack(btnPressed);
    }

    if (btnPressed && upPressed)
    {
        if (_txdata->waterjetAttack < 95)
            _txdata->waterjetAttack = _txdata->waterjetAttack + 5;
        _wjD->drawInfoboxWaterjetAttack(btnPressed);
    }
    if (btnPressed && downPressed)
    {
        if (_txdata->waterjetAttack > 0)
            _txdata->waterjetAttack = _txdata->waterjetAttack - 5;
        _wjD->drawInfoboxWaterjetAttack(btnPressed);
    }
}

/*
    incr/decr waterjetDecay if buttons are pressed
    and redraw infobox if needed
*/

void wjButton::waterjetDecayChange(bool upPressed, bool downPressed)
{
    static bool btnPressed = true;
    if (GO.BtnStart.isPressed() != 0 && !btnPressed)
    {
        // someone pressed the volume button
        btnPressed = true;
        _wjD->drawInfoboxWaterjetDecay(btnPressed);
    }
    if (GO.BtnStart.isReleased() != 0 && btnPressed)
    {
        // someone released the volume button
        btnPressed = false;
        _wjD->drawInfoboxWaterjetDecay(btnPressed);
    }

    if (btnPressed && upPressed)
    {
        if (_txdata->waterjetDecay < 95)
            _txdata->waterjetDecay = _txdata->waterjetDecay + 5;
        _wjD->drawInfoboxWaterjetDecay(btnPressed);
    }
    if (btnPressed && downPressed)
    {
        if (_txdata->waterjetDecay > 0)
            _txdata->waterjetDecay = _txdata->waterjetDecay - 5;
        _wjD->drawInfoboxWaterjetDecay(btnPressed);
    }
}

bool wjButton::upWasPressed(void)
{
    bool static btnUpPressed = false;
    if (GO.JOY_Y.isAxisPressed() == 2)
    {
        if (!btnUpPressed)
        {
            // detect rising edge on up button
            btnUpPressed = true;
            return true;
        }
    }
    else
    {
        btnUpPressed = false;
    }
    return false;
}

bool wjButton::downWasPressed(void)
{
    bool static btnDownPressed = false;
    if (GO.JOY_Y.isAxisPressed() == 1)
    {
        if (!btnDownPressed)
        {
            // detect rising edge on up button
            btnDownPressed = true;
            return true;
        }
    }
    else
    {
        btnDownPressed = false;
    }
    return false;
}

void wjButton::showInstructions(void)
{
    _wjD->drawInstructions();
    while (GO.BtnA.isPressed() == 0)
    {
        GO.update();
        delay(50);
    }
}
