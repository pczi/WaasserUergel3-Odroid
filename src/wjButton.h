// methods to manage all the buttons
// - menu + volume + select + start
// - up + down

#ifndef wjButton_h
#define wjButton_h

#include <odroid_go.h>
#include "wjGlobals.h"
#include "wjDisplay.h"

class wjButton
{
public:
    wjButton(wjGlobals *pgv, wjDisplay *pwjD, SEND_DATA_STRUCTURE *ptxdata);
    void bassBeatChange();
    void ampFactorChange(bool upPressed, bool downPressed);
    void waterjetAttackChange(bool upPressed, bool downPressed);
    void waterjetDecayChange(bool upPressed, bool downPressed);
    bool upWasPressed(void);
    bool downWasPressed(void);
    void showInstructions(void);

private:
    // dependency injection of the global variables
    wjGlobals *_gv;
    wjDisplay *_wjD;
    SEND_DATA_STRUCTURE *_txdata;
};

#endif