#pragma once

#include <LiquidCrystal_I2C.h>
class HMI
{
  private:
    enum State{ST_HOMEPAGE,ST_MENUPAGE,ST_SETSIZE,ST_CHANGENUM};
    //Objects
    LiquidCrystal_I2C* lcdPtr;
    Keypad* keypadPtr;
    State currentState;
    uint8_t currentRow; //LCD
    bool typingDone;
    //Parameter to configure on the device
    uint8_t cylinderSize;
    //Keypad input methods
    void StoreKey(char key,char* keyBuffer,uint8_t len);
    void SetParam(uint8_t* paramPtr,char* paramBuffer,uint8_t paramColumn);
    void SetMobileNum(char* numBuffer);
    //Parameter buffer
    char cylinderSizeBuff[3];
    
    void AlignData(uint8_t param);
    //Display Methods
    void HighlightRow(uint8_t row, char** heading);
    void DisplayRowHeadings(char** heading, bool num = false);
    void SetCylinderSizeMenu(void);
    void DisplayHomePage(void);
    void DisplayMenuPage(void);
    void ChangeMobileNumber(void);
    //State Methods
    void changeStateTo(State nextState);
    void StateFunc_Homepage(void);
    void StateFunc_Menupage(void);
    void StateFunc_SetSize(void);
    void StateFunc_ChangeNum(void);
    //void StateFunc_LeakageCheck(void);
     
  public:
    HMI(LiquidCrystal_I2C* lcdPtr,Keypad* keyPadPtr);
    char mobileNum[13];
    void Start(void);
    void ClearTypingDoneFlag(void);
    bool GetTypingDoneFlag(void);
};
