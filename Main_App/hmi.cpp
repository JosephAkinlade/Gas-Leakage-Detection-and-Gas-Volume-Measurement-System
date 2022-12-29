#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //Version 1.1.2
#include "keypad.h"
#include "hmi.h"

//LCD (indexes and number of rows)
const uint8_t minRow = 0;
const uint8_t maxRow = 3;
const uint8_t numOfRows = 4;

void HMI::AlignData(uint8_t param)
{
  if(param < 10)
  {
    lcdPtr->print('0');
  }
  lcdPtr->print(param);
}

void HMI::StoreKey(char key,char* keyBuffer,uint8_t len)
{
  for(uint8_t i = 0; i < len - 1 ;i++)
  {
    keyBuffer[i] = keyBuffer[i + 1];
  }
  keyBuffer[len - 1] = key;   
}

void HMI::SetParam(uint8_t* paramPtr,char* paramBuffer,uint8_t paramColumn)
{
  while(1)
  {
    char key = keypadPtr->GetChar();
    if(isdigit(key))
    {
      HMI::StoreKey(key,paramBuffer,2);
      //convert char buffer to int
      String str = String(paramBuffer);
      *paramPtr = str.toInt();
      lcdPtr->setCursor(paramColumn,currentRow);
      HMI::AlignData(*paramPtr);
    }
    else
    {
      if(key == '#')
      {
        break;
      }
    }
  }
}

void HMI::SetMobileNum(char* numBuffer)
{
 int i = 0;
 memset(numBuffer,' ',12);
  while(1)
  {
    char key = keypadPtr->GetChar();
    if(isdigit(key))
    {
      i++;
      if(i < 12)
      {
        numBuffer[i-1] = key;
        lcdPtr->setCursor(i,1);
        lcdPtr->print(key);
        
      }
    }
    else
    {
      if(key == '#')
      {
        lcdPtr->setCursor(0,0);
        lcdPtr->print(' ');
        typingDone = true;
        Serial.println(HMI::mobileNum);
        break;
      }
    }
  }    
}

void HMI::HighlightRow(uint8_t row,char** heading)
{
  for(uint8_t i = 0; i < numOfRows; i++)
  {
    if(row == i)
    {
      heading[i][0] = '-';
      break;
    }
  }  
}



void HMI::DisplayRowHeadings(char** heading, bool num = false)
{
  for(uint8_t i = 0; i < numOfRows; i++)
  {
    if(num)
    {
      lcdPtr->setCursor(1,i);
      lcdPtr->print(heading[i]);
    }
    else
    {
      lcdPtr->setCursor(0,i);
      lcdPtr->print(heading[i]);
    }
  }  
}

void HMI::DisplayHomePage(void)
{
  char heading1[] = "Cylinder Size: ";
  char heading2[] = "Gas left: ";
  char heading3[] = "Condition: ";
  char heading4[] = "";
  char* heading[] = {heading1,heading2,heading3,heading4};
  //HMI::HighlightRow(row,heading);
  HMI::DisplayRowHeadings(heading);
}

void HMI::DisplayMenuPage(void)
{
  char heading1[] = "1.Set Cylinder Size";
  char heading2[] = "2.User Mobile No.";
  char heading3[] = "";
  char heading4[] = "";
  char* heading[] = {heading1,heading2,heading3,heading4};
  //HMI::HighlightRow(row,heading);
  HMI::DisplayRowHeadings(heading);
}

void HMI::SetCylinderSizeMenu(void)
{
  char heading1[] = " Enter size(KG): ";
  char heading2[] = "";
  char heading3[] = "";
  char heading4[] = "";
  char* heading[] = {heading1,heading2,heading3,heading4};
  //HMI::HighlightRow(row,heading);
  HMI::DisplayRowHeadings(heading);  
}

void HMI::ChangeMobileNumber(void)
{
  char heading1[] = "Mobile No.: ";
  char* heading2 = HMI::mobileNum;
  char heading3[] = "";
  char heading4[] = "";
  char* heading[] = {heading1,heading2,heading3,heading4};
  HMI::DisplayRowHeadings(heading,true); 
}

void HMI::changeStateTo(State nextState)
{
  currentState = nextState;
  lcdPtr->clear();
}

void HMI::StateFunc_Homepage(void)
{
  //Get stored weight from EEPROM
  uint8_t weight = 12;
  HMI::DisplayHomePage();
  //Code to add the values

  char key = keypadPtr->GetChar();
  switch(key)
  {
    case 'A':
      HMI::changeStateTo(ST_MENUPAGE);
      break;
  }
}

void HMI::StateFunc_Menupage(void)
{
  HMI::DisplayMenuPage();
  char key = keypadPtr->GetChar();
  switch(key)
  {
    case '1':
      HMI::changeStateTo(ST_SETSIZE);
      break;
    case '2':
      HMI::changeStateTo(ST_CHANGENUM);
      break;
    case 'B':
      HMI::changeStateTo(ST_HOMEPAGE);
      break;
  }
}

void HMI::StateFunc_SetSize(void)
{
  HMI::SetCylinderSizeMenu();
  char key = keypadPtr->GetChar();
  switch(key)
  {
    case 'B':
      HMI::changeStateTo(ST_MENUPAGE);
      break;
    case '#':
      lcdPtr->setCursor(0,0);
      lcdPtr->print('>');
      HMI::SetParam(&cylinderSize,cylinderSizeBuff,17);
      break;
  } 
}

void HMI::StateFunc_ChangeNum(void)
{
  HMI::ChangeMobileNumber();
  char key = keypadPtr->GetChar();
  switch(key)
  {
    case 'B':
      HMI::changeStateTo(ST_MENUPAGE);
      break;
    case '#':
      lcdPtr->setCursor(0,0);
      lcdPtr->print('>');
      lcdPtr->setCursor(0,1);
      lcdPtr->print("            ");
      HMI::SetMobileNum(mobileNum);
      break;
  } 
}

HMI::HMI(LiquidCrystal_I2C* lcdPtr,Keypad* keypadPtr)
{
  //Initialize private variables
  this->lcdPtr = lcdPtr;
  this->keypadPtr = keypadPtr;
  currentState = ST_HOMEPAGE;
  currentRow = minRow;
  typingDone = false;
  
  cylinderSize = 0; 
  uint8_t i;
  for(i = 0; i < 2; i++)
  {
    cylinderSizeBuff[i] = '0';
 
  } 
  cylinderSizeBuff[i] = '\0'; 
}

void HMI::Start(void)
{
  switch(currentState)
  {
    case ST_HOMEPAGE:
      HMI::StateFunc_Homepage();
      break;
    case ST_MENUPAGE:
      HMI::StateFunc_Menupage();
      break;
    case ST_SETSIZE:
      HMI::StateFunc_SetSize();
      break;
    case ST_CHANGENUM:
      HMI:: StateFunc_ChangeNum();
      break;       
  }
}

void HMI::ClearTypingDoneFlag(void)
{
  typingDone = false;
}

bool HMI::GetTypingDoneFlag(void)
{
  return typingDone;
}
