#pragma once

#include <LiquidCrystal_I2C.h>
class HMI
{
  private:
    enum State{ST_HOMEPAGE,ST_MENUPAGE,ST_SETSIZE};
    //Objects
    LiquidCrystal_I2C* lcdPtr;
    Keypad* keypadPtr;
    State currentState;
    uint8_t currentRow; //LCD
    bool saveButtonPressed;
    //Parameter to configure on the device
    uint8_t cylinderSize;
    //Keypad input methods
    void StoreKey(char key,char* keyBuffer,uint8_t len);
    void SetParam(uint8_t* paramPtr,char* paramBuffer,uint8_t paramColumn);
    //Parameter buffer
    char cylinderSizeBuff[3];
    void AlignData(uint8_t param);
    //Display Methods
    void HighlightRow(uint8_t row, char** heading);
    void DisplayRowHeadings(char** heading);
    void setCylinderSizeMenu(void);
    void DisplayHomePage(void);
    void DisplayMenuPage();
    //State Methods
    void changeStateTo(State nextState);
    void StateFunc_Homepage(void);
    void StateFunc_Menupage(void);
    void StateFunc_SetSize(void);
    //void StateFunc_LeakageCheck(void);
     
  public:
    HMI(LiquidCrystal_I2C* lcdPtr,Keypad* keyPadPtr);
    void Start(void);
    void ClearSaveButtonState(void);
    bool GetSaveButtonState(void);
};
