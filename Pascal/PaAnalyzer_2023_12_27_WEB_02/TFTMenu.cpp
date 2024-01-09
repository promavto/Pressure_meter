#include "TFTMenu.h"

#ifdef USE_TFT_MODULE

#ifdef USE_BUZZER
#include "Buzzer.h"
#endif

#include "Settings.h"
#include "Sensor.h"


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define FONT_HEIGHT(dc) dc->fontHeight(1)

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static const char* WEEKDAYS[] = {

  "ПН",
  "ВТ",
  "СР",
  "ЧТ",
  "ПТ",
  "СБ",
  "ВС"

};

int variantPassword = 0;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
unsigned int utf8GetCharSize(unsigned char bt) 
{ 
  if (bt < 128) 
  return 1; 
  else if ((bt & 0xE0) == 0xC0) 
  return 2; 
  else if ((bt & 0xF0) == 0xE0) 
  return 3; 
  else if ((bt & 0xF8) == 0xF0) 
  return 4; 
  else if ((bt & 0xFC) == 0xF8) 
  return 5; 
  else if ((bt & 0xFE) == 0xFC) 
  return 6; 

 
  return 1; 
} 
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ButtonPressed(int btn)
{
  if(btn != -1)
  {
    #ifdef USE_BUZZER
    Buzzer.buzz();
    #endif
  }

  TFTScreen->onButtonPressed(btn);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ButtonReleased(int btn)
{
  TFTScreen->onButtonReleased(btn);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawButtonsYield() // вызывается после отрисовки каждой кнопки
{
  yield();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawScreenCaption(TFTMenu* hal, const String& str) // рисуем заголовок экрана
{

  TFT_Class* dc = hal->getDC();
  
  if(!dc)
  {
    return;
  }
  
  TFTRus* rusPrinter = hal->getRusPrinter();
  
  int screenWidth = dc->width();
  
  dc->setFreeFont(TFT_FONT);
  
  int fontHeight = FONT_HEIGHT(dc);
  int top = 10;

  // подложка под заголовок
  dc->fillRect(0, 0, screenWidth, top*2 + fontHeight, TFT_NAVY);
   
  int left = (screenWidth - rusPrinter->textWidth(str.c_str()))/2;

  rusPrinter->print(str.c_str(),left,top, TFT_NAVY, TFT_WHITE);    
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawTimeInBox(TFTInfoBox* box, uint32_t val)
{
  TFT_Class* dc = TFTScreen->getDC();
  
  if(!dc)
  {
    return;
  }
  
  TFTRus* rusPrinter = TFTScreen->getRusPrinter();
  
  TFTInfoBoxContentRect rc =  box->getContentRect(TFTScreen);
  dc->fillRect(rc.x,rc.y,rc.w,rc.h, INFO_BOX_BACK_COLOR);
  yield();

  dc->setFreeFont(SEVEN_SEG_NUM_FONT_PLUS);

  uint8_t hours = val/60;
  uint8_t mins = val%60;

  String strVal;
  if(hours < 10)
    strVal += '0';

  strVal += hours;
  strVal += ':';

  if(mins < 10)
    strVal += '0';

  strVal += mins;

  
  int fontHeight = FONT_HEIGHT(dc);
  int strLen = rusPrinter->textWidth(strVal.c_str());

  int leftPos = rc.x + (rc.w - strLen)/2;
  int topPos = rc.y + (rc.h - fontHeight)/2;
  rusPrinter->print(strVal.c_str(),leftPos,topPos,INFO_BOX_BACK_COLOR,SENSOR_BOX_FONT_COLOR);
  yield();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawValueInBox(TFTInfoBox* box, const String& strVal, FONTTYPE font)
{
  TFT_Class* dc = TFTScreen->getDC();
  
  if(!dc)
  {
    return;
  }
  
  TFTRus* rusPrinter = TFTScreen->getRusPrinter();
  
  TFTInfoBoxContentRect rc =  box->getContentRect(TFTScreen);
  dc->fillRect(rc.x,rc.y,rc.w,rc.h, INFO_BOX_BACK_COLOR);
  yield();

  dc->setFreeFont(font);

  
  int fontHeight = FONT_HEIGHT(dc);
  int strLen = rusPrinter->textWidth(strVal.c_str());

  int leftPos = rc.x + (rc.w - strLen)/2;
  int topPos = rc.y + (rc.h - fontHeight)/2;
  rusPrinter->print(strVal.c_str(),leftPos,topPos,INFO_BOX_BACK_COLOR,SENSOR_BOX_FONT_COLOR);
  yield();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawValueInBox(TFTInfoBox* box, int val)
{
  return drawValueInBox(box,String(val));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawValueInBox(TFTInfoBox* box, int16_t val)
{
  return drawValueInBox(box,String(val));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawValueInBox(TFTInfoBox* box, uint16_t val)
{
  return drawValueInBox(box,String(val));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawValueInBox(TFTInfoBox* box, int8_t val)
{
  return drawValueInBox(box,String(val));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawValueInBox(TFTInfoBox* box, uint8_t val)
{
  return drawValueInBox(box,String(val));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void drawValueInBox(TFTInfoBox* box, uint32_t val)
{
  return drawValueInBox(box,String(val));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTInfoBox
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTInfoBox::TFTInfoBox(const char* caption, int width, int height, int x, int y, int cxo)
{
  boxCaption = caption;
  boxWidth = width;
  boxHeight = height;
  posX = x;
  posY = y;
  captionXOffset = cxo;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTInfoBox::~TFTInfoBox()
{
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTInfoBox::drawCaption(TFTMenu* menuManager, const char* caption)
{
  TFT_Class* dc = menuManager->getDC();
  if(!dc)
  {
    return;
  }  
  
  dc->setFreeFont(TFT_FONT);
  
  menuManager->getRusPrinter()->print(caption,posX+captionXOffset,posY,TFT_BACK_COLOR,INFO_BOX_CAPTION_COLOR);  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTInfoBox::draw(TFTMenu* menuManager)
{
  drawCaption(menuManager,boxCaption);
  
  int curTop = posY;

  TFT_Class* dc = menuManager->getDC();
  if(!dc)
  {
    return;
  }

  dc->setFreeFont(TFT_FONT);
    
  int fontHeight = FONT_HEIGHT(dc);
  
  curTop += fontHeight + INFO_BOX_CONTENT_PADDING;

  dc->fillRoundRect(posX, curTop, boxWidth, (boxHeight - fontHeight - INFO_BOX_CONTENT_PADDING),2,INFO_BOX_BACK_COLOR);

  yield();

  dc->drawRoundRect(posX, curTop, boxWidth, (boxHeight - fontHeight - INFO_BOX_CONTENT_PADDING),2,INFO_BOX_BORDER_COLOR);

  yield();
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTInfoBoxContentRect TFTInfoBox::getContentRect(TFTMenu* menuManager)
{
    TFTInfoBoxContentRect result;
    TFT_Class* dc = menuManager->getDC();
	
	if(!dc)
	{
		return result;
	}	

    dc->setFreeFont(TFT_FONT);
    
    int fontHeight = FONT_HEIGHT(dc);

    result.x = posX + INFO_BOX_CONTENT_PADDING;
    result.y = posY + fontHeight + INFO_BOX_CONTENT_PADDING*2;

    result.w = boxWidth - INFO_BOX_CONTENT_PADDING*2;
    result.h = boxHeight - (fontHeight + INFO_BOX_CONTENT_PADDING*3);

    return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractTFTScreen::AbstractTFTScreen()
{ 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractTFTScreen::~AbstractTFTScreen()
{ 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTMenu
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTMenu* TFTScreen = NULL;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTMenu::TFTMenu()
{
  TFTScreen = this;
  currentScreenIndex = -1;
  flags.isLCDOn = true;
  switchTo = NULL;
  switchToIndex = -1;
  tftDC = NULL;
  tftTouch = NULL;
  on_action = NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::setup()
{
  int rot = 1;
  int dRot = 1;

  
  tftDC = new TFT_eSPI();

	tftDC->init();
	tftDC->setRotation(dRot);
	tftDC->fillScreen(TFT_BACK_COLOR);

	tftDC->setFreeFont(TFT_FONT);

	tftDC->setTextColor(TFT_RED, TFT_BACK_COLOR);


	tftTouch = tftDC;

    delay(200);
    Settings.displayBacklight(true); // включаем подсветку

  TFTCalibrationData data = Settings.GetTftCalibrationData();
  if(data.isValid)
  {
    tftTouch->setTouch(data.points);
  }
  else
  {
    uint16_t dt[5] = {304, 3502, 280, 3507, 4};
    tftTouch->setTouch(dt);
  }

	tftTouch->setRotation(rot);
	tftTouch->begin();


  rusPrint.init(tftDC);

  
  resetIdleTimer();

  //// добавляем служебные экраны

  // окно сообщения
  TFTScreenInfo mbscrif;
    
  //TFTTouchCalibrationScreen
  mbscrif.screen = new TFTTouchCalibrationScreen();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "TOUCH_CALIBRATION";
  screens.push_back(mbscrif);

  //TFTMenuScreen
  mbscrif.screen = new TFTMenuScreen();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "MENU";
  screens.push_back(mbscrif);

  //TFTMeasureScreen
  mbscrif.screen = new TFTMeasureScreen();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "MEASURE";
  screens.push_back(mbscrif);

  //TFTCalMeasureScreen
  mbscrif.screen = new TFTRefMeasureScreen(false);
  mbscrif.screen->setup(this);
  mbscrif.screenName = "REF_SCREEN";
  screens.push_back(mbscrif);

  //TFTCalibrationScreen
  mbscrif.screen = new TFTCalibrationScreen();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "CALIBRATION";
  screens.push_back(mbscrif);

  //TFTServiceMenuScreen
  mbscrif.screen = new TFTServiceMenuScreen();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "SERVICE_MENU";
  screens.push_back(mbscrif);

  //TFTTimeSettingsMenuScreen
  mbscrif.screen = new TFTTimeSettingsMenuScreen();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "TIME_SETTINGS";
  screens.push_back(mbscrif);

  //TFTMeasureSettingsScreen
  mbscrif.screen = new TFTMeasureSettingsScreen();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "MEASURE_SETTINGS";
  screens.push_back(mbscrif);

  //TFTCalibrationSettingsScreen
  mbscrif.screen = new TFTCalibrationSettingsScreen();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "CALIBRATION_SETTINGS";
  screens.push_back(mbscrif);


  mbscrif.screen = MessageBoxScreen::create();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "MB";
  screens.push_back(mbscrif);

   //TFTSetTimeLedLCDOff
  mbscrif.screen = new TFTSetTimeLedLCDOff();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "SetTimeLedLCDOff";
  screens.push_back(mbscrif);

  //TFTSetTimePowerOff
  mbscrif.screen = new TFTSetTimePowerOff();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "SetTimePowerOff";
  screens.push_back(mbscrif);
  
  //TFTSetDateTimeMenuScreen
  mbscrif.screen = new TFTSetDateTimeMenuScreen();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "SET_DATE_TIME";
  screens.push_back(mbscrif);

  //TFTSetTimeMenuScreen
  mbscrif.screen = new TFTSetTimeMenuScreen();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "SET_TIME";
  screens.push_back(mbscrif);

  //TFTSetDateMenuScreen
  mbscrif.screen = new TFTSetDateMenuScreen();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "SET_DATE";
  screens.push_back(mbscrif);

  

/*
  // клавиатура
  mbscrif.screen = KeyboardScreen::create();
  mbscrif.screen->setup(this);
  mbscrif.screenName = "KBD";
  screens.push_back(mbscrif);
*/
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::onButtonPressed(int button)
{
  if(currentScreenIndex == -1)
    return;

  resetIdleTimer();
  TFTScreenInfo* currentScreenInfo = &(screens[currentScreenIndex]);
  currentScreenInfo->screen->onButtonPressed(this, button);

  if(on_action != NULL)
  {
    on_action(currentScreenInfo->screen);
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::onButtonReleased(int button)
{
  if(currentScreenIndex == -1)
    return;

  resetIdleTimer();
  TFTScreenInfo* currentScreenInfo = &(screens[currentScreenIndex]);
  currentScreenInfo->screen->onButtonReleased(this, button);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::update()
{
  if(!tftDC)
  {
    return;
  }
	
  if(currentScreenIndex == -1 && !switchTo)                         // ни разу не рисовали ещё ничего, исправляемся
  {
     TFTCalibrationData dt = Settings.GetTftCalibrationData();
     
      if(dt.isValid)
      {
        //#ifdef ENABLE_CALIBRATION_AT_START                      // калибровка при старте включена
        //// калибровка тачскрина есть, проверяем, есть ли сохранённые калибровочные коэффициенты для 20.9% О2
        //if(Settings.Get20_9_O2Value() < 1)                      // нет коэффициента, измеряем при старте
        //{
        //  switchToScreen("CALIBRATION");          
        //}
        //else // есть коэффициент, считаем, что калибровка не нужна
        //#endif // ENABLE_CALIBRATION_AT_START
        //{
          switchToScreen("MENU");
        //}
      }
      else
      {
        switchToScreen("TOUCH_CALIBRATION");
      }
  }

  if(switchTo != NULL)
  {
      tftDC->fillScreen(TFT_BACK_COLOR); // clear screen first      
      yield();
      currentScreenIndex = switchToIndex;
      switchTo->onActivate(this);
      switchTo->update(this);
      yield();
      switchTo->draw(this);
      yield();
      resetIdleTimer(); // сбрасываем таймер ничегонеделанья

      switchTo = NULL;
      switchToIndex = -1;
    return;
  }



  // обновляем текущий экран
  TFTScreenInfo* currentScreenInfo = &(screens[currentScreenIndex]);
  currentScreenInfo->screen->update(this);
  yield();
  
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractTFTScreen* TFTMenu::getScreen(const char* screenName)
{
  for(size_t i=0;i<screens.size();i++)
  {
    TFTScreenInfo* si = &(screens[i]);
    if(!strcmp(si->screenName,screenName))
    {
      return si->screen;
    }
  }

  return NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::switchToScreen(AbstractTFTScreen* to)
{
	if(!tftDC)
	{
		return;
	}
   // переключаемся на запрошенный экран
  for(size_t i=0;i<screens.size();i++)
  {
    TFTScreenInfo* si = &(screens[i]);
    if(si->screen == to)
    {
      switchTo = si->screen;
      switchToIndex = i;
      break;

    }
  } 
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::switchToScreen(const char* screenName)
{
	if(!tftDC)
	{
		return;
	}
  
  // переключаемся на запрошенный экран
  for(size_t i=0;i<screens.size();i++)
  {
    TFTScreenInfo* si = &(screens[i]);
    if(!strcmp(si->screenName,screenName))
    {
      switchTo = si->screen;
      switchToIndex = i;
      break;

    }
  }

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractTFTScreen* TFTMenu::getActiveScreen()
{
  if(currentScreenIndex > -1 && screens.size())
  {
    TFTScreenInfo* currentScreenInfo = &(screens[currentScreenIndex]);
     return (currentScreenInfo->screen);
  }  
  
  return NULL;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMenu::resetIdleTimer()
{
  idleTimer = millis();

  if(currentScreenIndex > -1 && screens.size() && on_action != NULL)
  {
    TFTScreenInfo* currentScreenInfo = &(screens[currentScreenIndex]);
    on_action(currentScreenInfo->screen);
  }
}


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTMeasureScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTMeasureScreen::TFTMeasureScreen()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTMeasureScreen::~TFTMeasureScreen()
{
  delete screenButtons;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMeasureScreen::setup(TFTMenu* menuManager)
{
    TFT_Class* dc = menuManager->getDC();
   screenButtons = new TFT_Buttons_Rus(dc, menuManager->getTouch(), menuManager->getRusPrinter());
   screenButtons->setTextFont(TFT_FONT);
   screenButtons->setSymbolFont(SENSOR_FONT);

   screenButtons->setButtonColors(TFT_CHANNELS_BUTTON_COLORS);

   TFTRus* rusPrinter = menuManager->getRusPrinter();

   int screenWidth = dc->height();
   int screenHeight = dc->width();
 
   dc->setFreeFont(TFT_FONT);
   int textFontHeight = FONT_HEIGHT(dc);
   int textFontWidth = dc->textWidth("w", 1);

   const int BUTTON_WIDTH = 220;
   const int BUTTON_HEIGHT = 60;
   const int V_SPACING = 10;

   int left = (screenWidth - BUTTON_WIDTH)/2;
   int top = (screenHeight - (BUTTON_HEIGHT*2 + V_SPACING*2));

   repeatButton = screenButtons->addButton(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, REPEAT_CAPTION);
   top += BUTTON_HEIGHT + V_SPACING;
   backButton = screenButtons->addButton(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, WM_BACK_CAPTION);
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMeasureScreen::update(TFTMenu* menuManager)
{

  if(timerEnabled)
  {
      if(millis() - timer >= 1000ul)
      {
        menuManager->resetIdleTimer(); // сбрасываем таймер ничегонеделанья, чтобы не переключилось на главный экран
        
        timer = millis();
        measureTime--;
        drawMeasureTime(menuManager,TFT_FONT_COLOR);
        if(!measureTime)
        {
          timerEnabled = false;

		  delay(300);

          // измерение закончено, просим датчик рассчитать значение
          SensorData percents = Sensor.compute();

          // стираем сообщение
          drawMessage(menuManager,TFT_BACK_COLOR);

          // стираем время отсчёта
          drawMeasureTime(menuManager,TFT_BACK_COLOR);
    
          // выводим значение на экран
          String data;
          data += percents.Value;
          data += '.';
          if(percents.Fract < 10)
            data += '0';
          data += percents.Fract;
          
          drawSensorData(menuManager,data, percents.HasValue);
		  uint16_t val = Sensor.read();
		  draw_mv(menuManager, val);
          
          // рисуем кнопки
          //screenButtons->drawButtons(drawButtonsYield);
          screenButtons->showButton(repeatButton,true);
          screenButtons->relabelButton(backButton, WM_BACK_CAPTION, true);
                
        }
      }
  }

    int pressed_button = screenButtons->checkButtons(ButtonPressed, ButtonReleased);
    if(pressed_button != -1)
    {
       
      if(pressed_button == backButton)
      {
          menuManager->switchToScreen("MENU");
      }
      else if(pressed_button == repeatButton)
      {
        menuManager->switchToScreen("MEASURE");
      }
    }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMeasureScreen::onActivate(TFTMenu* menuManager)
{
  measureTime = Settings.GetMeasureTime();
  timer = millis();
  lastTextWidth = -1;
  timerEnabled = true;
  screenButtons->relabelButton(backButton, STOP_CAPTION);
  screenButtons->hideButton(repeatButton);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMeasureScreen::drawSensorData(TFTMenu* menuManager, String& data, bool hasValue)
{
  TFT_Class* dc = menuManager->getDC();
  TFTRus* rusPrinter = menuManager->getRusPrinter();

  dc->setFreeFont(SEVEN_SEG_NUM_FONT_MDS);
  int screenWidth = dc->width();
  int screenHeight = dc->height();
  int fontHeight = FONT_HEIGHT(dc);

  int top = 40;

  if(!hasValue)
  {
    data = "ERR";
  }
    
  int tw = rusPrinter->textWidth(data.c_str());

  dc->setFreeFont(SENSOR_FONT);
  int percentWidth = rusPrinter->textWidth("0");

  if(!hasValue)
  {
    percentWidth = 0;
  }
  
  int left = (screenWidth - (tw + percentWidth) )/2;

  dc->setFreeFont(SEVEN_SEG_NUM_FONT_MDS);
  rusPrinter->print(data.c_str(), left, top, TFT_BACK_COLOR, TFT_FONT_COLOR);

  if(hasValue)
  {
    dc->setFreeFont(SENSOR_FONT);
    left += tw;
    rusPrinter->print("0", left, top, TFT_BACK_COLOR, TFT_FONT_COLOR);
  }
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMeasureScreen::drawMeasureTime(TFTMenu* menuManager, uint16_t color)
{
  TFT_Class* dc = menuManager->getDC();
  TFTRus* rusPrinter = menuManager->getRusPrinter();

  dc->setFreeFont(SEVEN_SEG_NUM_FONT_MDS);
  int screenWidth = dc->width();
  int screenHeight = dc->height();
  int fontHeight = FONT_HEIGHT(dc);

  int top = 40;

  String str;
  str = measureTime;
  int tw = rusPrinter->textWidth(str.c_str());
  if(lastTextWidth > tw)
  {
    int lastLeft = (screenWidth - lastTextWidth)/2;
    dc->fillRect(lastLeft,top,lastTextWidth,fontHeight,TFT_BACK_COLOR);
  }

  lastTextWidth = tw;
  int left = (screenWidth - tw)/2;

  rusPrinter->print(str.c_str(), left, top, TFT_BACK_COLOR, color);
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMeasureScreen::drawMessage(TFTMenu* menuManager, uint16_t color)
{
  TFT_Class* dc = menuManager->getDC();
  TFTRus* rusPrinter = menuManager->getRusPrinter();

  dc->setFreeFont(TFT_FONT);
  int fontHeight = FONT_HEIGHT(dc);
  int screenWidth = dc->width();
  int screenHeight = dc->height();
  const int v_spacing = 2;

  Vector<const char*> lines;
  lines.push_back("ПОДОЖДИТЕ,");
  lines.push_back("ИДЁТ");
  lines.push_back("ИЗМЕРЕНИЕ");

  int top = (screenHeight - lines.size()*(fontHeight+v_spacing))/2 + 20;

  for(size_t i=0;i<lines.size();i++)
  {
      int left = (screenWidth - rusPrinter->textWidth(lines[i]))/2;

      rusPrinter->print(lines[i], left, top, TFT_BACK_COLOR, color);

      top += fontHeight + v_spacing;
  }  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTMeasureScreen::draw(TFTMenu* menuManager)
{
  // рисуем кнопки
  screenButtons->drawButtons(drawButtonsYield);

  drawMessage(menuManager,TFT_FONT_COLOR);
  drawMeasureTime(menuManager,TFT_FONT_COLOR);
  
}

void TFTMeasureScreen::draw_mv(TFTMenu* menuManager, uint16_t val)
{
	TFT_Class* dc = menuManager->getDC();
	TFTRus* rusPrinter = menuManager->getRusPrinter();

	String data; data = val;


	dc->setFreeFont(TFT_FONT);
	int screenWidth = dc->width();
	int screenHeight = dc->height();
	int fontHeight = FONT_HEIGHT(dc);

	data += " mv";

	int top = 5;

	int tw = rusPrinter->textWidth(data.c_str());

	int left = (screenWidth - tw) / 2;

	rusPrinter->print(data.c_str(), left, top, TFT_BACK_COLOR, TFT_BLACK);

	dc->drawLine(10, 25, 229, 25, TFT_RED);

}




//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTRefMeasureScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTRefMeasureScreen::TFTRefMeasureScreen(bool _ref_enable)
{
    ref_enable = _ref_enable;
	waitMessage_enable = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTRefMeasureScreen::~TFTRefMeasureScreen()
{
    delete screenButtons;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTRefMeasureScreen::setup(TFTMenu* menuManager)
{

    waitMessage.push_back("УСТАНОВИТЕ");
    waitMessage.push_back("ПЕРЕМЫЧКУ");
    waitMessage.push_back("ВМЕСТО ДАТЧИКА");
    waitMessage.push_back("ДАВЛЕНИЯ");

    doneMessage.push_back("ИЗМЕРЕНИЕ");
    doneMessage.push_back("ЗАВЕРШЕНО");

    TFT_Class* dc = menuManager->getDC();
    screenButtons = new TFT_Buttons_Rus(dc, menuManager->getTouch(), menuManager->getRusPrinter());
    screenButtons->setTextFont(TFT_FONT);
    screenButtons->setSymbolFont(SENSOR_FONT);

    screenButtons->setButtonColors(TFT_CHANNELS_BUTTON_COLORS);

    TFTRus* rusPrinter = menuManager->getRusPrinter();

    int screenWidth = dc->width();
    int screenHeight = dc->height();

    dc->setFreeFont(TFT_FONT);
    int textFontHeight = FONT_HEIGHT(dc);
    int textFontWidth = dc->textWidth("w", 1);

    const int BUTTON_WIDTH = 220;
    const int BUTTON_HEIGHT = 40;
    const int V_SPACING = 10;

    int left = (screenWidth - BUTTON_WIDTH) / 2;
    int top = (screenHeight - (BUTTON_HEIGHT * 2 + V_SPACING * 2));

    startRepeatButton = screenButtons->addButton(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, START_CAPTION);
    top += BUTTON_HEIGHT + V_SPACING;
    backButton = screenButtons->addButton(left, top, BUTTON_WIDTH, BUTTON_HEIGHT, WM_BACK_CAPTION);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTRefMeasureScreen::update(TFTMenu* menuManager)
{

     // стираем сообщение
    if (lastMessage && waitMessage_enable)
    {
        drawMessage(menuManager, *lastMessage, TFT_BACK_COLOR);
        lastMessage = NULL;
    }

	if (waitMessage_enable == true)
	{
		refADCVal = Settings.GetReference_voltage();

		drawSensorData(menuManager, refADCVal, TFT_FONT_COLOR);

		drawMessage(menuManager, waitMessage, TFT_FONT_COLOR);
		waitMessage_enable = false;
	}
 
    int pressed_button = screenButtons->checkButtons(ButtonPressed, ButtonReleased);

    if (pressed_button != -1)
    {
        if (pressed_button == backButton)
        {
			waitMessage_enable = true;
			menuManager->switchToScreen("CAL_SETTINGS");// ("SERVICE_MENU");
        }
        else if (pressed_button == startRepeatButton)
        {

			//delay(300); // немного подождем 
			refADCVal = Sensor.readRef();
			Serial.print("Sensor readRef ... ");
			Serial.println(refADCVal);

			Settings.SetReference_voltage(refADCVal);
			waitMessage_enable = true;
			// стираем сообщение
			if (lastMessage && waitMessage_enable)
			{
				drawMessage(menuManager, *lastMessage, TFT_BACK_COLOR);
				lastMessage = NULL;
			}

			if (waitMessage_enable == true)
			{
				refADCVal = Settings.GetReference_voltage();
				Serial.print("Sensor Get ... ");
				Serial.println(refADCVal);
				drawSensorData(menuManager, refADCVal, TFT_FONT_COLOR);
				drawMessage(menuManager, doneMessage, TFT_FONT_COLOR);
				waitMessage_enable = false;
			}
        }
    }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTRefMeasureScreen::onActivate(TFTMenu* menuManager)
{
    measureTime = Settings.GetCalibrationTime();

    lastMessage = NULL;
    lastTextWidth = -1;
    timerEnabled = false;
    refADCVal = 0;

    screenButtons->relabelButton(startRepeatButton, START_CAPTION);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTRefMeasureScreen::drawSensorData(TFTMenu* menuManager, int32_t val, uint16_t color)
{
    TFT_Class* dc = menuManager->getDC();
    TFTRus* rusPrinter = menuManager->getRusPrinter();

    String data; 
	data = val;


    dc->setFreeFont(SEVEN_SEG_NUM_FONT_MDS);
    int screenWidth = dc->width();
    int screenHeight = dc->height();
    int fontHeight = FONT_HEIGHT(dc);


    int top = 50;

    int tw = rusPrinter->textWidth(data.c_str());

    int left = (screenWidth - tw) / 2;
    dc->fillRect(0, top - 15, 239, top + 15, TFT_BACK_COLOR);
    rusPrinter->print(data.c_str(), left, top, TFT_BACK_COLOR, color);

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTRefMeasureScreen::drawMessage(TFTMenu* menuManager, Vector<const char*>& lines, uint16_t color)
{
    TFT_Class* dc = menuManager->getDC();
    TFTRus* rusPrinter = menuManager->getRusPrinter();

    dc->setFreeFont(TFT_FONT);
    int fontHeight = FONT_HEIGHT(dc);
    int screenWidth = dc->width();
    const int v_spacing = 2;

    int top = 110;

    for (size_t i = 0; i < lines.size(); i++)
    {
        int left = (screenWidth - rusPrinter->textWidth(lines[i])) / 2;

        rusPrinter->print(lines[i], left, top, TFT_BACK_COLOR, color);

        top += fontHeight + v_spacing;
    }

    lastMessage = &lines;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTRefMeasureScreen::draw(TFTMenu* menuManager)
{
   // drawScreenCaption(menuManager, cal209 ? CAL_209_CAPTION : CAL_0_CAPTION);
    // рисуем кнопки
    screenButtons->drawButtons(drawButtonsYield);

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTCalibrationScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTCalibrationScreen::TFTCalibrationScreen()
{
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTCalibrationScreen::~TFTCalibrationScreen()
{
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTCalibrationScreen::setup(TFTMenu* menuManager)
{
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTCalibrationScreen::update(TFTMenu* menuManager)
{
  if(timerEnabled)
  {
      if(millis() - timer >= 1000ul)
      {
        menuManager->resetIdleTimer(); // сбрасываем таймер ничегонеделанья, чтобы не переключилось на главный экран
        
        timer = millis();
        measureTime--;
        drawMeasureTime(menuManager);
        if(!measureTime)
        {
          timerEnabled = false;

          //ЗАНОСИМ В ПАМЯТЬ ЗНАЧЕНИЕ С АНАЛОГОВОГО ВХОДА

		  uint16_t adcVal = 0;

		  adcVal = Sensor.read();

         // Settings.Set20_9_O2Value(adcVal);
                
          menuManager->switchToScreen("MENU");
        }
      }
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTCalibrationScreen::onActivate(TFTMenu* menuManager)
{
  measureTime = Settings.GetCalibrationTime();
  timer = millis();
  lastTextWidth = -1;
  timerEnabled = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTCalibrationScreen::drawMeasureTime(TFTMenu* menuManager)
{
  TFT_Class* dc = menuManager->getDC();
  TFTRus* rusPrinter = menuManager->getRusPrinter();

  dc->setFreeFont(SEVEN_SEG_NUM_FONT_MDS);
  int screenWidth = dc->width();
  int screenHeight = dc->height();
  int fontHeight = FONT_HEIGHT(dc);

  int top = 40;

  String str;
  str = measureTime;
  int tw = rusPrinter->textWidth(str.c_str());
  if(lastTextWidth > tw)
  {
    int lastLeft = (screenWidth - lastTextWidth)/2;
    dc->fillRect(lastLeft,top,lastTextWidth,fontHeight,TFT_BACK_COLOR);
  }

  lastTextWidth = tw;
  int left = (screenWidth - tw)/2;

  rusPrinter->print(str.c_str(), left, top, TFT_BACK_COLOR, TFT_FONT_COLOR);
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTCalibrationScreen::draw(TFTMenu* menuManager)
{

  TFT_Class* dc = menuManager->getDC();
  TFTRus* rusPrinter = menuManager->getRusPrinter();

  dc->setFreeFont(TFT_FONT);
  int fontHeight = FONT_HEIGHT(dc);
  int screenWidth = dc->width();
  int screenHeight = dc->height();

  Vector<const char*> lines;
  lines.push_back("ПОДОЖДИТЕ,");
  lines.push_back("ИДЁТ");
  lines.push_back("КАЛИБРОВКА");

  int top = (screenHeight - lines.size()*fontHeight)/2 + 20;

  for(size_t i=0;i<lines.size();i++)
  {
      int left = (screenWidth - rusPrinter->textWidth(lines[i]))/2;

      rusPrinter->print(lines[i], left, top, TFT_BACK_COLOR, TFT_FONT_COLOR);

      top += fontHeight;
  }

  drawMeasureTime(menuManager);

}
///------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTTouchCalibrationScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTTouchCalibrationScreen* TouchCalibrationScreen = NULL;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTTouchCalibrationScreen::TFTTouchCalibrationScreen()
{
	canSwitch = false;
  TouchCalibrationScreen = this;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TFTTouchCalibrationScreen::~TFTTouchCalibrationScreen()
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTTouchCalibrationScreen::setup(TFTMenu* menuManager)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTTouchCalibrationScreen::update(TFTMenu* menuManager)
{
  menuManager->resetIdleTimer();                         // сбрасываем таймер ничегонеделанья, чтобы не переключилось на главный экран
  
	if (canSwitch)
	{
		canSwitch = false;
    
    #ifdef ENABLE_CALIBRATION_AT_START                   // калибровка датчика при старте включена
		//if (Settings.Get20_9_O2Value() < 1)              // нет коэффициента, измеряем при старте
		//{
		//	menuManager->switchToScreen("CALIBRATION");
		//}
		//else                                             // есть коэффициент, считаем, что калибровка не нужна
    #endif                                               // ENABLE_CALIBRATION_AT_START
		//{                   
			menuManager->switchToScreen("MENU");
		//}
	}
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TFTTouchCalibrationScreen::draw(TFTMenu* menuManager)
{

	TFT_Class* dc = menuManager->getDC();
	TFTRus* rusPrinter = menuManager->getRusPrinter();

	dc->setFreeFont(TFT_SMALL_FONT);
	dc->fillScreen(TFT_BLACK);
	int fontHeight = FONT_HEIGHT(dc);
	int screenWidth = dc->width();
	int screenHeight = dc->height();
	const int v_spacing = 2;

	Vector<const char*> lines;
	lines.push_back("ТРЕБУЕТСЯ КАЛИБРОВКА ТАЧСКРИНА.");
	lines.push_back("");
	lines.push_back("НАЖИМАЙТЕ ПООЧЕРЁДНО НА УГЛЫ.");

	int top = (screenHeight - lines.size()*(fontHeight + v_spacing)) / 2;

	for (size_t i = 0; i < lines.size(); i++)
	{
		int left = (screenWidth - rusPrinter->textWidth(lines[i])) / 2;

		rusPrinter->print(lines[i], left, top, TFT_BLACK, TFT_WHITE);

		top += fontHeight + v_spacing;
	}
	delay(400);
	TFTCalibrationData calData;

	dc->calibrateTouch(calData.points, TFT_WHITE, TFT_BLACK, 30);
	dc->setTouch(calData.points);
	Settings.SetTftCalibrationData(calData);

	dc->setFreeFont(TFT_FONT);
	canSwitch = true;

}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTTimeSettingsMenuScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTTimeSettingsMenuScreen::TFTTimeSettingsMenuScreen()
 {
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTTimeSettingsMenuScreen::~TFTTimeSettingsMenuScreen()
 {
   delete screenButtons;  
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTTimeSettingsMenuScreen::onActivate(TFTMenu* menuManager)
 {

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTTimeSettingsMenuScreen::setup(TFTMenu* menuManager)
 {

   TFT_Class* dc = menuManager->getDC();

   if (!dc)
   {
     return;
   }

   screenButtons = new TFT_Buttons_Rus(dc, menuManager->getTouch(), menuManager->getRusPrinter());
   screenButtons->setTextFont(TFT_FONT);
   screenButtons->setSymbolFont(SENSOR_FONT);

   screenButtons->setButtonColors(TFT_CHANNELS_BUTTON_COLORS);

   int screenWidth = dc->width();
   int screenHeight = dc->height();

  const int v_spacing = 10;
  const int h_spacing = 5;

  // у нас 4 кнопки
  int button_width = screenWidth - h_spacing*2;
  int button_height = (screenHeight - v_spacing*6)/5;
  int left = h_spacing;
  int top = v_spacing;

  measureTimeButton = screenButtons->addButton(left, top, button_width, button_height, MEASURE_TIME_CAPTION);
  top += v_spacing + button_height;

  calibrationTimeButton = screenButtons->addButton(left, top, button_width, button_height, CAL_TIME_CAPTION);
  top += v_spacing + button_height;

  ledLCDTimeButton = screenButtons->addButton(left, top, button_width, button_height, CAL_TIME_LCD_OFF);
  top += v_spacing + button_height;

  powerOffTimeButton = screenButtons->addButton(left, top, button_width, button_height, CAL_TIME_POWER_OFF);
  top += v_spacing + button_height;

  backButton  = screenButtons->addButton(left, top, button_width, button_height, WM_BACK_CAPTION);

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  void TFTTimeSettingsMenuScreen::update(TFTMenu* menuManager)
 {
  int pressed_button = screenButtons->checkButtons(ButtonPressed, ButtonReleased);
  if(pressed_button != -1)
  {
    if(pressed_button == backButton)
    {
      menuManager->switchToScreen("SERVICE_MENU");
    }
    else
    if(pressed_button == measureTimeButton)
    {
      menuManager->switchToScreen("MEASURE_SETTINGS");
    }
    else
    if(pressed_button == calibrationTimeButton)
    {
      menuManager->switchToScreen("CALIBRATION_SETTINGS");
    }
    else
    if (pressed_button == ledLCDTimeButton)
    {
        menuManager->switchToScreen("SetTimeLedLCDOff");
    }
    else
    if (pressed_button == powerOffTimeButton)
    {
        menuManager->switchToScreen("SetTimePowerOff");
    }
    
  }

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTTimeSettingsMenuScreen::draw(TFTMenu* menuManager)
 {

   if (screenButtons)
   {
     screenButtons->drawButtons(drawButtonsYield);
   }

 }
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTServiceMenuScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTServiceMenuScreen::TFTServiceMenuScreen()
 {
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTServiceMenuScreen::~TFTServiceMenuScreen()
 {
   delete screenButtons;  
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTServiceMenuScreen::onActivate(TFTMenu* menuManager)
 {

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTServiceMenuScreen::setup(TFTMenu* menuManager)
 {


   TFT_Class* dc = menuManager->getDC();

   if (!dc)
   {
     return;
   }

   screenButtons = new TFT_Buttons_Rus(dc, menuManager->getTouch(), menuManager->getRusPrinter());
   screenButtons->setTextFont(TFT_FONT);
   screenButtons->setSymbolFont(SENSOR_FONT);

   screenButtons->setButtonColors(TFT_CHANNELS_BUTTON_COLORS);

   int screenWidth = dc->width();
   int screenHeight = dc->height();

  const int v_spacing = 8;
  const int h_spacing = 5;

  // у нас 4 кнопки
  int button_width = screenWidth - h_spacing*2 - 40;
  int button_height = (screenHeight - v_spacing*7)/6;
  int left = h_spacing+20;
  int top = v_spacing;

 

  setDataButton = screenButtons->addButton(left, top, button_width, button_height, SET_DATA_TIME);      //Кнопка "УСТ.ДАТА/ВРЕМЯ"
  top += v_spacing + button_height;

  calButton = screenButtons->addButton(left, top, button_width, button_height, CAL_CAPTION);             //Кнопка "КАЛИБРОВКА"
  top += v_spacing + button_height;

  koeff_Pa_Button = screenButtons->addButton(left, top, button_width, button_height, SET_KOEFF_PA);      //Кнопка "КОЕФФИЦИЕНТ Рa" 
  top += v_spacing + button_height;

  FreeButton2 = screenButtons->addButton(left, top, button_width, button_height, FREE_CAPTION);           //Кнопка  
  top += v_spacing + button_height;

  FreeButton3 = screenButtons->addButton(left, top, button_width, button_height, FREE_CAPTION);          //Кнопка  
  top += v_spacing + button_height;

  backButton  = screenButtons->addButton(left, top, button_width, button_height, WM_BACK_CAPTION);       //Кнопка "< НАЗАД"

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  void TFTServiceMenuScreen::update(TFTMenu* menuManager)
 {
  int pressed_button = screenButtons->checkButtons(ButtonPressed, ButtonReleased);
  if(pressed_button != -1)
  {
    if(pressed_button == backButton)
    {
      menuManager->switchToScreen("MENU");
    }
    else
    if(pressed_button == koeff_Pa_Button)
    {
      //menuManager->switchToScreen("VARIANT_PASSWORD");
    }
    else
    if(pressed_button == FreeButton2)
    {
      //menuManager->switchToScreen("PASSWORD_TIME");
    }
    else 
    if(pressed_button == FreeButton3)
    {
     // menuManager->switchToScreen("SET_ATMOSFERA");
    }
	else
	if (pressed_button == setDataButton)
	{
		menuManager->switchToScreen("SET_DATE_TIME");
	}
	else
	if (pressed_button == calButton)
	{
		menuManager->switchToScreen("CAL_SETTINGS");
	}


    
  }

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTServiceMenuScreen::draw(TFTMenu* menuManager)
 {

   if (screenButtons)
   {
     screenButtons->drawButtons(drawButtonsYield);
   }

 }
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTCalibrationSettingsScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTCalibrationSettingsScreen::TFTCalibrationSettingsScreen()
 {
  tickerButton = -1;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTCalibrationSettingsScreen::~TFTCalibrationSettingsScreen()
 {
   delete screenButtons;
   delete settingsBox;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTCalibrationSettingsScreen::onActivate(TFTMenu* menuManager)
 {
    setting = Settings.GetCalibrationTime();
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTCalibrationSettingsScreen::setup(TFTMenu* menuManager)
 {
   TFT_Class* dc = menuManager->getDC();

   if (!dc)
   {
     return;
   }

   screenButtons = new TFT_Buttons_Rus(dc, menuManager->getTouch(), menuManager->getRusPrinter());
   screenButtons->setTextFont(TFT_FONT);
   screenButtons->setSymbolFont(SENSOR_FONT);

   screenButtons->setButtonColors(TFT_CHANNELS_BUTTON_COLORS);

   int screenWidth = dc->width();
   int screenHeight = dc->height();

   const int v_spacing = 5;

   int button_width = screenWidth - v_spacing*2;
   int button_height = 60;

   int left = v_spacing;
   int top = 42;

   upButton = screenButtons->addButton(left, top, button_width, button_height, "c");
   screenButtons->setButtonFont(upButton,VARIOUS_SYMBOLS_32x32);
   top += button_height + v_spacing;


   dc->setFreeFont(TFT_FONT);
   int fontHeight = FONT_HEIGHT(dc);
  
   settingsBox  = new TFTInfoBox("", button_width, 100, left, top - fontHeight - INFO_BOX_CONTENT_PADDING);   
   top += button_height + v_spacing*4;

   downButton = screenButtons->addButton(left, top, button_width, button_height, "d");
   screenButtons->setButtonFont(downButton,VARIOUS_SYMBOLS_32x32);
   

   backButton = screenButtons->addButton(left, screenHeight - button_height - v_spacing, button_width, button_height, WM_BACK_CAPTION);

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  void TFTCalibrationSettingsScreen::onButtonPressed(TFTMenu* menuManager, int buttonID)
 {
   tickerButton = -1;

   if(buttonID == upButton || buttonID == downButton)
   {
      tickerButton = buttonID;
      Ticker.start(this);
   }
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTCalibrationSettingsScreen::onButtonReleased(TFTMenu* menuManager, int buttonID)
 {
   Ticker.stop();
   tickerButton = -1;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTCalibrationSettingsScreen::incSetting(int val)
 {
    int32_t old = setting;
    
    setting += val;
    if(setting < 1)
    {
      setting = 1;
    }
    if(setting > MAX_TIME_VALUE)
    {
      setting = MAX_TIME_VALUE;
    }

    if(old != setting)
    {
      drawValueInBox(settingsBox, String(setting));
    }
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTCalibrationSettingsScreen::onTick()
 {
    TFTScreen->resetIdleTimer(); // сбрасываем таймер ничегонеделанья, чтобы не переключилось на главный экран
  
   if (tickerButton == upButton)
     incSetting(3);
   else
   if (tickerButton == downButton)
     incSetting(-3);
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  void TFTCalibrationSettingsScreen::update(TFTMenu* menuManager)
 {
   if (!menuManager->getDC())
   {
     return;
   }

  int pressed_button = screenButtons->checkButtons(ButtonPressed, ButtonReleased);
  if(pressed_button != -1)
  {
    if(pressed_button == backButton)
    {
      Settings.SetCalibrationTime(setting);
      menuManager->switchToScreen("TIME_SETTINGS");
    }
    else
    if(pressed_button == upButton)
    {
       incSetting(1);
    }
    else
    if(pressed_button == downButton)
    {
       incSetting(-1);
    }
  }

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTCalibrationSettingsScreen::draw(TFTMenu* menuManager)
 {

     drawScreenCaption(menuManager,CAL_TIME_CAPTION);

     settingsBox->draw(menuManager);
     drawValueInBox(settingsBox, String(setting));
     
     screenButtons->drawButtons(drawButtonsYield);

 }
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTMeasureSettingsScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTMeasureSettingsScreen::TFTMeasureSettingsScreen()
 {
  tickerButton = -1;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTMeasureSettingsScreen::~TFTMeasureSettingsScreen()
 {
   delete screenButtons;
   delete settingsBox;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMeasureSettingsScreen::onActivate(TFTMenu* menuManager)
 {
    setting = Settings.GetMeasureTime();
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMeasureSettingsScreen::setup(TFTMenu* menuManager)
 {
   TFT_Class* dc = menuManager->getDC();

   if (!dc)
   {
     return;
   }

   screenButtons = new TFT_Buttons_Rus(dc, menuManager->getTouch(), menuManager->getRusPrinter());
   screenButtons->setTextFont(TFT_FONT);
   screenButtons->setSymbolFont(SENSOR_FONT);

   screenButtons->setButtonColors(TFT_CHANNELS_BUTTON_COLORS);

   int screenWidth = dc->width();
   int screenHeight = dc->height();

   const int v_spacing = 5;

   int button_width = screenWidth - v_spacing*2;
   int button_height = 60;

   int left = v_spacing;
   int top = 42;

   upButton = screenButtons->addButton(left, top, button_width, button_height, "c");
   screenButtons->setButtonFont(upButton,VARIOUS_SYMBOLS_32x32);
   top += button_height + v_spacing;


   dc->setFreeFont(TFT_FONT);
   int fontHeight = FONT_HEIGHT(dc);
  
   settingsBox  = new TFTInfoBox("", button_width, 100, left, top - fontHeight - INFO_BOX_CONTENT_PADDING);   
   top += button_height + v_spacing*4;

   downButton = screenButtons->addButton(left, top, button_width, button_height, "d");
   screenButtons->setButtonFont(downButton,VARIOUS_SYMBOLS_32x32);
   

   backButton = screenButtons->addButton(left, screenHeight - button_height - v_spacing, button_width, button_height, WM_BACK_CAPTION);

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  void TFTMeasureSettingsScreen::onButtonPressed(TFTMenu* menuManager, int buttonID)
 {
   tickerButton = -1;

   if(buttonID == upButton || buttonID == downButton)
   {
      tickerButton = buttonID;
      Ticker.start(this);
   }
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMeasureSettingsScreen::onButtonReleased(TFTMenu* menuManager, int buttonID)
 {
   Ticker.stop();
   tickerButton = -1;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMeasureSettingsScreen::incSetting(int val)
 {
    int32_t old = setting;
    
    setting += val;
    if(setting < 1)
    {
      setting = 1;
    }
    if(setting > MAX_TIME_VALUE)
    {
      setting = MAX_TIME_VALUE;
    }

    if(old != setting)
    {
      drawValueInBox(settingsBox, String(setting));
    }
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMeasureSettingsScreen::onTick()
 {
    TFTScreen->resetIdleTimer(); // сбрасываем таймер ничегонеделанья, чтобы не переключилось на главный экран
  
   if (tickerButton == upButton)
     incSetting(3);
   else
   if (tickerButton == downButton)
     incSetting(-3);
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  void TFTMeasureSettingsScreen::update(TFTMenu* menuManager)
 {
   if (!menuManager->getDC())
   {
     return;
   }

  int pressed_button = screenButtons->checkButtons(ButtonPressed, ButtonReleased);
  if(pressed_button != -1)
  {
    if(pressed_button == backButton)
    {
      Settings.SetMeasureTime(setting);
      menuManager->switchToScreen("TIME_SETTINGS");
    }
    else
    if(pressed_button == upButton)
    {
       incSetting(1);
    }
    else
    if(pressed_button == downButton)
    {
       incSetting(-1);
    }
  }

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMeasureSettingsScreen::draw(TFTMenu* menuManager)
 {

     drawScreenCaption(menuManager,MEASURE_TIME_CAPTION);

     settingsBox->draw(menuManager);
     drawValueInBox(settingsBox, String(setting));
     
     screenButtons->drawButtons(drawButtonsYield);

 }


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTSetTimeLedLCDOff
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTSetTimeLedLCDOff::TFTSetTimeLedLCDOff()
 {
     stage = 0;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTSetTimeLedLCDOff::~TFTSetTimeLedLCDOff()
 {
     delete screenButtons;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimeLedLCDOff::setup(TFTMenu* menuManager)
 {
     TFT_Class* dc = menuManager->getDC();
     screenButtons = new TFT_Buttons_Rus(dc, menuManager->getTouch(), menuManager->getRusPrinter());
     screenButtons->setTextFont(TFT_FONT);
     screenButtons->setSymbolFont(SENSOR_FONT);

     screenButtons->setButtonColors(TFT_CHANNELS_BUTTON_COLORS);

     TFTRus* rusPrinter = menuManager->getRusPrinter();

     int screenWidth = dc->width();
     int screenHeight = dc->height();

     dc->setFreeFont(TFT_FONT);
     int textFontHeight = FONT_HEIGHT(dc);

     // создаём кнопки клавиатуры
     static const char* captions[] = {
      "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "C", ENTER_CAPTION
     };

     const int v_spacing = 5;
     const int buttons_in_row = 3;
     int button_width = (screenWidth - v_spacing * (1 + buttons_in_row)) / buttons_in_row;
     int button_height = textFontHeight * 2 + v_spacing * 2 + 4;
     int top = button_height + v_spacing * 3;
     int left = v_spacing;

     int row_cntr = 0;
     lastKeyButtonID = 0;
     for (int i = 0; i < sizeof(captions) / sizeof(captions[0]); i++)
     {
         if (row_cntr >= buttons_in_row)
         {
             row_cntr = 0;
             left = v_spacing;
             top += button_height + v_spacing;
         }

         lastKeyButtonID = screenButtons->addButton(left, top, button_width, button_height, captions[i]);
         left += v_spacing + button_width;
         row_cntr++;
     }

     int small_button_width = button_width;

     // добавляем текс-бокс
     textBox = screenButtons->addButton(v_spacing, v_spacing, screenWidth - v_spacing * 2, button_height, "");
     screenButtons->disableButton(textBox);
     screenButtons->setButtonInactiveFontColor(textBox, TFT_FONT_COLOR);

     // добавляем кнопку "Назад"
     button_width = screenWidth - v_spacing * 2;

     left = (screenWidth - button_width) / 2;
     top = (screenHeight - (button_height * 2 + v_spacing * 2));

     top += button_height + v_spacing;
     backButton = screenButtons->addButton(left, top, button_width, button_height, WM_BACK_CAPTION);

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimeLedLCDOff::update(TFTMenu* menuManager)
 {

     int pressed_button = screenButtons->checkButtons(ButtonPressed, ButtonReleased);
     if (pressed_button != -1)
     {
         if (pressed_button <= lastKeyButtonID) // кнопки клавиатуры
         {
             if (!strcmp(screenButtons->getLabel(pressed_button), ENTER_CAPTION))
             {
                 if (enteredTimeLCD.length() > 0)
                 {
                     // Сохранить 
                     int intVar;
                     intVar = enteredTimeLCD.toInt();
                     Settings.SetTimeLedLCD(intVar);
                 }
                 else
                 {
                     stage = 1;
                     enteredTimeLCD = "";
                     relabelStageMessage(true); // пишем приглашение
                 }
             }
             else if (!strcmp(screenButtons->getLabel(pressed_button), "C"))  // Очистить поле ввода данных
             {
                 enteredTimeLCD = "";
                 relabelStageMessage(true); // пишем приглашение
             }
             else if (enteredTimeLCD.length() < MAX_TIME_LCD_LENGTH) // защита от длинного ввода времени отключения подсветки дисплея
             {
                 enteredTimeLCD += screenButtons->getLabel(pressed_button);  // Отобразить новые данные
                 screenButtons->relabelButton(textBox, enteredTimeLCD.c_str(), true);
             }
         }
         else
         {
             // другие кнопки

             if (pressed_button == backButton)
             {
                 menuManager->switchToScreen("TIME_SETTINGS");
             }
         }
     }
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimeLedLCDOff::relabelStageMessage(bool redraw) // Приглашение к вводу новых данных
 {
     const char* message = ">Задержка сек.";
     switch (stage)
     {
     case 0: break;
     case 1: message = "> Введите данные"; break;
     }

     screenButtons->relabelButton(textBox, message, redraw);
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimeLedLCDOff::onActivate(TFTMenu* menuManager)  // Отобразить текущие данные при запуске функции
 {
     enteredTimeLCD = "";
     int time_LCD = Settings.GetTimeLedLCD();
     enteredTimeLCD = String(time_LCD, DEC);
     stage = 0;
     screenButtons->relabelButton(textBox, enteredTimeLCD.c_str(), true);
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimeLedLCDOff::draw(TFTMenu* menuManager)
 {

     // рисуем кнопки
     screenButtons->drawButtons(drawButtonsYield);

 }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTSetTimePowerOff
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTSetTimePowerOff::TFTSetTimePowerOff()
 {
     stage = 0;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTSetTimePowerOff::~TFTSetTimePowerOff()
 {
     delete screenButtons;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimePowerOff::setup(TFTMenu* menuManager)
 {
     TFT_Class* dc = menuManager->getDC();
     screenButtons = new TFT_Buttons_Rus(dc, menuManager->getTouch(), menuManager->getRusPrinter());
     screenButtons->setTextFont(TFT_FONT);
     screenButtons->setSymbolFont(SENSOR_FONT);

     screenButtons->setButtonColors(TFT_CHANNELS_BUTTON_COLORS);

     TFTRus* rusPrinter = menuManager->getRusPrinter();

     int screenWidth = dc->width();
     int screenHeight = dc->height();

     dc->setFreeFont(TFT_FONT);
     int textFontHeight = FONT_HEIGHT(dc);

     // создаём кнопки клавиатуры
     static const char* captions[] = {
      "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "C", ENTER_CAPTION
     };

     const int v_spacing = 5;
     const int buttons_in_row = 3;
     int button_width = (screenWidth - v_spacing * (1 + buttons_in_row)) / buttons_in_row;
     int button_height = textFontHeight * 2 + v_spacing * 2 + 4;
     int top = button_height + v_spacing * 3;
     int left = v_spacing;

     int row_cntr = 0;
     lastKeyButtonID = 0;
     for (int i = 0; i < sizeof(captions) / sizeof(captions[0]); i++)
     {
         if (row_cntr >= buttons_in_row)
         {
             row_cntr = 0;
             left = v_spacing;
             top += button_height + v_spacing;
         }

         lastKeyButtonID = screenButtons->addButton(left, top, button_width, button_height, captions[i]);
         left += v_spacing + button_width;
         row_cntr++;
     }

     int small_button_width = button_width;

     // добавляем текс-бокс
     textBox = screenButtons->addButton(v_spacing, v_spacing, screenWidth - v_spacing * 2, button_height, "");
     screenButtons->disableButton(textBox);
     screenButtons->setButtonInactiveFontColor(textBox, TFT_FONT_COLOR);

     // добавляем кнопку "Назад"
     button_width = screenWidth - v_spacing * 2;

     left = (screenWidth - button_width) / 2;
     top = (screenHeight - (button_height * 2 + v_spacing * 2));

     top += button_height + v_spacing;
     backButton = screenButtons->addButton(left, top, button_width, button_height, WM_BACK_CAPTION);

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimePowerOff::update(TFTMenu* menuManager)
 {

     int pressed_button = screenButtons->checkButtons(ButtonPressed, ButtonReleased);
     if (pressed_button != -1)
     {
         if (pressed_button <= lastKeyButtonID) // кнопки клавиатуры
         {
             if (!strcmp(screenButtons->getLabel(pressed_button), ENTER_CAPTION))
             {
                 if (enteredPowerOff.length() > 0)
                 {
                     // Сохранить 
                     int intVar;
                     intVar = enteredPowerOff.toInt();
                     Settings.SetTimePowerOff(intVar);
                 }
                 else
                 {
                     stage = 1;
					 enteredPowerOff = "";
                     relabelStageMessage(true); // пишем приглашение
                 }
             }
             else if (!strcmp(screenButtons->getLabel(pressed_button), "C"))
             {
				 enteredPowerOff = "";
                 relabelStageMessage(true); // пишем приглашение
             }
             else if (enteredPowerOff.length() < MAX_TIME_POWER_LENGTH) // защита от длинного ввода таймера отключения питания
             {
				 enteredPowerOff += screenButtons->getLabel(pressed_button);
                 screenButtons->relabelButton(textBox, enteredPowerOff.c_str(), true);
             }
         }
         else
         {
             // другие кнопки

             if (pressed_button == backButton)
             {
                 menuManager->switchToScreen("TIME_SETTINGS");
             }
         }
     }
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimePowerOff::relabelStageMessage(bool redraw)
 {
     const char* message = ">Задержка сек.";
     switch (stage)
     {
     case 0: break;
     case 1: message = "> Введите данные"; break;
     }

     screenButtons->relabelButton(textBox, message, redraw);
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimePowerOff::onActivate(TFTMenu* menuManager)
 {
	 enteredPowerOff = "";
     int PowerOff = Settings.GetTimePowerOff();
	 enteredPowerOff = String(PowerOff, DEC);
     stage = 0;
     screenButtons->relabelButton(textBox, enteredPowerOff.c_str(), true);
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimePowerOff::draw(TFTMenu* menuManager)
 {

     // рисуем кнопки
     screenButtons->drawButtons(drawButtonsYield);

 }


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTSetDateTimeMenuScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTSetDateTimeMenuScreen::TFTSetDateTimeMenuScreen()
 {
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTSetDateTimeMenuScreen::~TFTSetDateTimeMenuScreen()
 {
	 delete screenButtons;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetDateTimeMenuScreen::onActivate(TFTMenu* menuManager)
 {

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetDateTimeMenuScreen::setup(TFTMenu* menuManager)
 {

	 TFT_Class* dc = menuManager->getDC();

	 if (!dc)
	 {
		 return;
	 }

	 screenButtons = new TFT_Buttons_Rus(dc, menuManager->getTouch(), menuManager->getRusPrinter());
	 screenButtons->setTextFont(TFT_FONT);
	 screenButtons->setSymbolFont(SENSOR_FONT);

	 screenButtons->setButtonColors(TFT_BUTTON_COLORS_BLUE);
	
	 int screenWidth = dc->width();
	 int screenHeight = dc->height();

	 const int v_spacing = 10;
	 const int h_spacing = 5;

	 // у нас 3 кнопки
	 int button_width = screenWidth - h_spacing * 2;
	 int button_height = (screenHeight - v_spacing * 5) / 4;
	 int left = h_spacing;
	 int top = v_spacing;


	 setTimeButton = screenButtons->addButton(left, top, button_width, button_height, SET_TIME);
	 top += v_spacing + button_height;

	 setDateButton = screenButtons->addButton(left, top, button_width, button_height, SET_DATA);
	 top += v_spacing + button_height;

	 top += v_spacing + button_height;

	 backButton = screenButtons->addButton(left, top, button_width, button_height, WM_BACK_CAPTION);

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetDateTimeMenuScreen::update(TFTMenu* menuManager)
 {


	 int pressed_button = screenButtons->checkButtons(ButtonPressed, ButtonReleased);
	 if (pressed_button != -1)
	 {
		if (pressed_button == backButton)
		{
			menuManager->switchToScreen("SERVICE_MENU");
		}
		else
		if (pressed_button == setTimeButton)
		{
			menuManager->switchToScreen("SET_TIME");
		}
		else
		if (pressed_button == setDateButton)
		{
			menuManager->switchToScreen("SET_DATE");
		}

}

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetDateTimeMenuScreen::draw(TFTMenu* menuManager)
 {
	 TFT_Class* dc = menuManager->getDC();

	 if (!dc)
	 {
		 return;
	 }

	 dc->fillScreen(TFT_BLACK);

	 if (screenButtons)
	 {
		 screenButtons->drawButtons(drawButtonsYield);
	 }

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTTimeSettingsMenuScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTSetTimeMenuScreen::TFTSetTimeMenuScreen()
 {
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTSetTimeMenuScreen::~TFTSetTimeMenuScreen()
 {
	 delete screenButtons;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimeMenuScreen::onActivate(TFTMenu* menuManager)
 {

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimeMenuScreen::setup(TFTMenu* menuManager)
 {

	 TFT_Class* dc = menuManager->getDC();

	 if (!dc)
	 {
		 return;
	 }

	 screenButtons = new TFT_Buttons_Rus(dc, menuManager->getTouch(), menuManager->getRusPrinter());
	 screenButtons->setTextFont(TFT_FONT);
	 screenButtons->setSymbolFont(SENSOR_FONT);

	 screenButtons->setButtonColors(TFT_BUTTON_COLORS_BLUE);

	 screenButtons->setSymbolFont(VARIOUS_SYMBOLS_32x32);


	 int screenWidth = dc->width();   // Ширина экрана
	 int screenHeight = dc->height(); // Высота экрана

	 int v_spacing = 20;              // Отступ слева
	 int h_spacing = 20;              // Отступ сверху
	 int between_buttons_w = 20;      // Расстояние между кнопками по горизонтали
	 int between_buttons_h = 30;      // Расстояние между кнопками по вертикали

	 // у нас 4 кнопки
	 int button_width = v_spacing;
	 int button_height = h_spacing;
	 int left = 20;
	 int top = 180;

	 int set_size_Button = (screenWidth - (v_spacing *2) - (between_buttons_w *2))/ 3;   // размер кнопок по высоте и ширине

	
	 up_hour_Button = screenButtons->addButton(button_width, button_height, set_size_Button, set_size_Button, "c", BUTTON_SYMBOL);
	 button_width += v_spacing + set_size_Button;
	 up_minute_Button = screenButtons->addButton(button_width, button_height, set_size_Button, set_size_Button, "c", BUTTON_SYMBOL);
	 button_width += v_spacing + set_size_Button;
	 up_second_Button = screenButtons->addButton(button_width, button_height, set_size_Button, set_size_Button, "c", BUTTON_SYMBOL);

	 button_width = v_spacing;
	 button_height += between_buttons_h + set_size_Button; // положение второго ряда кнопок сверху

	 down_hour_Button = screenButtons->addButton(button_width, button_height, set_size_Button, set_size_Button, "d", BUTTON_SYMBOL);
	 button_width += v_spacing + set_size_Button;
	 down_minute_Button = screenButtons->addButton(button_width, button_height, set_size_Button, set_size_Button, "d", BUTTON_SYMBOL);
	 button_width += v_spacing + set_size_Button;
	 down_second_Button = screenButtons->addButton(button_width, button_height, set_size_Button, set_size_Button, "d", BUTTON_SYMBOL);

	 button_width = v_spacing;
	 button_height += between_buttons_h + set_size_Button; // положение остальных кнопок сверху
	 h_spacing = 10;

	 int set_size_Button_w = (screenWidth - (v_spacing * 2));   // размер кнопок по ширине
	 int set_size_Button_h = (screenHeight - (h_spacing * 2)- button_height) / 2;   // размер кнопок по высоте и ширине
	 screenButtons->setTextFont(TFT_FONT);
	 
	 saveTimeButton = screenButtons->addButton(v_spacing, button_height, set_size_Button_w, set_size_Button_h, WM_SAVE_CAPTION);
	 button_height += h_spacing + set_size_Button_h;
	 backButton = screenButtons->addButton(v_spacing, button_height, set_size_Button_w, set_size_Button_h, WM_BACK_CAPTION);

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimeMenuScreen::update(TFTMenu* menuManager)
 {
	 TFT_Class* dc = menuManager->getDC();

	 if (!dc)
	 {
		 return;
	 }

	 TFTRus* rusPrinter = menuManager->getRusPrinter();
	 // setTextFont(TFT_FONT);
	 dc->setFreeFont(TFT_FONT);
	 dc->setTextColor(TFT_WHITE);


	 RTC_DS3231  rtc = Settings.GetClock();
	 RTCTime  dt = rtc.getTime();



	 int num_w_h = 33;
	 int num_w_m = 105;
	 int num_w_s = 177;
	 int num_h = 80;

	int pressed_button = screenButtons->checkButtons(ButtonPressed, ButtonReleased);
	if (pressed_button != -1)
	{
		if (pressed_button == backButton)
		{
			menuManager->switchToScreen("SET_DATE_TIME");
		}
		else if (pressed_button == saveTimeButton)
		{
			rtc.adjust(DateTime(t_temp_year, t_temp_mon, t_temp_date, t_temp_hour, t_temp_min, t_temp_sec));
		}
		else if (pressed_button == up_hour_Button)
		{
			rusPrinter->print("     ", num_w_h+10, num_h, TFT_BLACK, TFT_WHITE);
			t_temp_hour += 1;
			if (t_temp_hour == 24)
				t_temp_hour = 0;
			if (t_temp_hour < 10)
			{
				dc->drawNumber(0, num_w_h, num_h);
				dc->drawNumber(t_temp_hour, num_w_h + 14, num_h);
			}
			else
			{
				dc->drawNumber(t_temp_hour, num_w_h, num_h);
			}
		}
		else if (pressed_button == up_minute_Button)
		{
			rusPrinter->print("     ", num_w_m+10, num_h, TFT_BLACK, TFT_WHITE);
			t_temp_min += 1;
			if (t_temp_min == 60)
				t_temp_min = 0;
			if (t_temp_min < 10)
			{
				dc->drawNumber(0, num_w_m, num_h);
				dc->drawNumber(t_temp_min, num_w_m + 14, num_h);
			}
			else
			{
				dc->drawNumber(t_temp_min, num_w_m, num_h);
			}
		}
		else if (pressed_button == up_second_Button)
		{
			 rusPrinter->print("      ", num_w_s, num_h, TFT_BLACK, TFT_WHITE);
			t_temp_sec ++;
			if (t_temp_sec == 60)
				t_temp_sec = 0;
			if (t_temp_sec < 10)
			{
				dc->drawNumber(0, num_w_s, num_h);
				dc->drawNumber(t_temp_sec, num_w_s + 14, num_h);
			}
			else
			{
				dc->drawNumber(t_temp_sec, num_w_s, num_h);
			}
		}
		else if (pressed_button == down_hour_Button)
		{
			 rusPrinter->print("      ", num_w_h+10, num_h, TFT_BLACK, TFT_WHITE);
			t_temp_hour -= 1;
			if (t_temp_hour < 0 || t_temp_hour == 255)
				t_temp_hour = 23;

			if (t_temp_hour < 10)
			{
				dc->drawNumber(0, num_w_h, num_h);
				dc->drawNumber(t_temp_hour, num_w_h + 14, num_h);
			}
			else
			{
				dc->drawNumber(t_temp_hour, num_w_h, num_h);
			}
		}
		else if (pressed_button == down_minute_Button)
		{
			 rusPrinter->print("      ", num_w_m, num_h, TFT_BLACK, TFT_WHITE);
			t_temp_min -= 1;
			if (t_temp_min < 0 || t_temp_min == 255)
				t_temp_min = 59;
			if (t_temp_min < 10)
			{
				dc->drawNumber(0, num_w_m, num_h);
				dc->drawNumber(t_temp_min, num_w_m + 14, num_h);
			}
			else
			{
				dc->drawNumber(t_temp_min, num_w_m, num_h);
			}
		}
		else if (pressed_button == down_second_Button)
		{
			rusPrinter->print("      ", num_w_s, num_h, TFT_BLACK, TFT_WHITE);
			t_temp_sec --;
			if (t_temp_sec < 0 || t_temp_sec == 255)
				t_temp_sec = 59;

			if (t_temp_sec < 10)
			{
				dc->drawNumber(0, num_w_s, num_h);
				dc->drawNumber(t_temp_sec, num_w_s + 14, num_h);
			}
			else
			{
				dc->drawNumber(t_temp_sec, num_w_s, num_h);
			}
		}
	}
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetTimeMenuScreen::draw(TFTMenu* menuManager)
 {
	 TFT_Class* dc = menuManager->getDC();

	 if (!dc)
	 {
		 return;
	 }


#ifdef USE_DS3231_REALTIME_CLOCK

	 TFTRus* rusPrinter = menuManager->getRusPrinter();
	// setTextFont(TFT_FONT);
	 dc->setFreeFont(TFT_FONT);
	 dc->fillScreen(TFT_BLACK);
	 dc->setTextColor(TFT_WHITE);

	 rusPrinter->print("Установка времени",5,1,TFT_BLACK, TFT_WHITE);
	 rusPrinter->print("ЧАС", 25, 162, TFT_BLACK, TFT_WHITE);
	 rusPrinter->print("МИН.", 97, 162, TFT_BLACK, TFT_WHITE);
	 rusPrinter->print("СЕК.", 170, 162, TFT_BLACK, TFT_WHITE);

	 RTC_DS3231  rtc = Settings.GetClock();
	 // DateTime dt = rtc.now();

	 RTCTime  dt = rtc.getTime();


	
	  int num_w = 33;
	  int num_h = 80;
	 // получаем текущую дату
	 t_temp_date = dt.dayOfMonth;
	 t_temp_mon = dt.month;
	 t_temp_year = dt.year;
	 t_temp_dow = dt.dayOfWeek;
	 t_temp_hour = dt.hour;
	 t_temp_min = dt.minute;
	 t_temp_sec = dt.second;

	 if (t_temp_date == 0)
	 {
		 t_temp_date = 16;
		 t_temp_mon = 4;
		 t_temp_year = 22;
		 t_temp_dow = 6;
		 t_temp_hour = 9;
		 t_temp_min = 0;
		 t_temp_sec = 0;
	 }


	 if (t_temp_hour < 10)
	 {
		 dc->drawNumber(0, num_w, num_h);
		 dc->drawNumber(t_temp_hour, num_w+14, num_h);
	 }
	 else
	 {
		 dc->drawNumber(t_temp_hour, num_w, num_h);
	 }
	 if (t_temp_min < 10)
	 {
	     num_w += 72;
		 dc->drawNumber(0, num_w, num_h);
		 dc->drawNumber(t_temp_min, num_w+14, num_h);
	 }
	 else
	 {
	    num_w += 72;
		 dc->drawNumber(t_temp_min, num_w, num_h);
	 }
	 if (t_temp_sec < 10)
	 {
	  num_w += 72;
		 dc->drawNumber(0, num_w, num_h);
		 dc->drawNumber(t_temp_sec, num_w+14, num_h);
	 }
	 else
	 {
	     num_w += 72;
		 dc->drawNumber(t_temp_sec, num_w, num_h);
	 }

#endif 
	 if (screenButtons)
	 {
		 screenButtons->drawButtons(drawButtonsYield);
	 }
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTSetDateMenuScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTSetDateMenuScreen::TFTSetDateMenuScreen()
 {
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTSetDateMenuScreen::~TFTSetDateMenuScreen()
 {
	 delete screenButtons;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetDateMenuScreen::onActivate(TFTMenu* menuManager)
 {

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetDateMenuScreen::setup(TFTMenu* menuManager)
 {
	 TFT_Class* dc = menuManager->getDC();

	 if (!dc)
	 {
		 return;
	 }

	 screenButtons = new TFT_Buttons_Rus(dc, menuManager->getTouch(), menuManager->getRusPrinter());
	 screenButtons->setTextFont(TFT_FONT);
	 screenButtons->setSymbolFont(SENSOR_FONT);

	 screenButtons->setButtonColors(TFT_BUTTON_COLORS_BLUE);

	 screenButtons->setSymbolFont(VARIOUS_SYMBOLS_32x32);


	 int screenWidth = dc->width();   // Ширина экрана
	 int screenHeight = dc->height(); // Высота экрана

	 int v_spacing = 20;          // Отступ слева
	 int h_spacing = 20;          // Отступ сверху
	 int between_buttons_w = 20;  // Расстояние между кнопками по горизонтали
	 int between_buttons_h = 30;  // Расстояние между кнопками по вертикали

	 // у нас 4 кнопки
	 int button_width = v_spacing;
	 int button_height = h_spacing;
	 int left = 20;
	 int top = 180;

	 int set_size_Button = (screenWidth - (v_spacing * 2) - (between_buttons_w * 2)) / 3;   // размер кнопок по высоте и ширине

	 up_date_Button = screenButtons->addButton(button_width, button_height, set_size_Button, set_size_Button, "c", BUTTON_SYMBOL);
	 button_width += v_spacing + set_size_Button;
	 up_mon_Button = screenButtons->addButton(button_width, button_height, set_size_Button, set_size_Button, "c", BUTTON_SYMBOL);
	 button_width += v_spacing + set_size_Button;
	 up_year_Button = screenButtons->addButton(button_width, button_height, set_size_Button, set_size_Button, "c", BUTTON_SYMBOL);

	 button_width = v_spacing;
	 button_height += between_buttons_h + set_size_Button; // положение второго ряда кнопок сверху

	 down_date_Button = screenButtons->addButton(button_width, button_height, set_size_Button, set_size_Button, "d", BUTTON_SYMBOL);
	 button_width += v_spacing + set_size_Button;
	 down_mon_Button = screenButtons->addButton(button_width, button_height, set_size_Button, set_size_Button, "d", BUTTON_SYMBOL);
	 button_width += v_spacing + set_size_Button;
	 down_year_Button = screenButtons->addButton(button_width, button_height, set_size_Button, set_size_Button, "d", BUTTON_SYMBOL);

	 button_width = v_spacing;
	 button_height += between_buttons_h + set_size_Button; // положение остальных кнопок сверху
	 h_spacing = 10;

	 int set_size_Button_w = (screenWidth - (v_spacing * 2));   // размер кнопок по ширине
	 int set_size_Button_h = (screenHeight - (h_spacing * 2) - button_height) / 2;   // размер кнопок по высоте и ширине
	 screenButtons->setTextFont(TFT_FONT);

	 saveDateButton = screenButtons->addButton(v_spacing, button_height, set_size_Button_w, set_size_Button_h, WM_SAVE_CAPTION);
	 button_height += h_spacing + set_size_Button_h;
	 backButton = screenButtons->addButton(v_spacing, button_height, set_size_Button_w, set_size_Button_h, WM_BACK_CAPTION);

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetDateMenuScreen::update(TFTMenu* menuManager)
 {
	 TFT_Class* dc = menuManager->getDC();

	 if (!dc)
	 {
		 return;
	 }

	 TFTRus* rusPrinter = menuManager->getRusPrinter();
	 // setTextFont(TFT_FONT);
	 dc->setFreeFont(TFT_FONT);
	 dc->setTextColor(TFT_WHITE);


	 RTC_DS3231  rtc = Settings.GetClock();
	 RTCTime  dt = rtc.getTime();


	 int num_w_d   = 33;
	 int num_w_mon = 105;
	 int num_w_y   = 177;
	 int num_h     = 80;

	int pressed_button = screenButtons->checkButtons(ButtonPressed, ButtonReleased);
	if (pressed_button != -1)
	{
		if (pressed_button == backButton)
		{
			menuManager->switchToScreen("SET_DATE_TIME");
		}
		else
		if (pressed_button == saveDateButton)
		{
			rtc.adjust(DateTime(t_temp_year, t_temp_mon, t_temp_date, t_temp_hour, t_temp_min, t_temp_sec));
		}
		else if (pressed_button == up_date_Button)
		{
			rusPrinter->print("     ", num_w_d+10, num_h, TFT_BLACK, TFT_WHITE);
			t_temp_date += 1;
			t_temp_date = validateDate(t_temp_date, t_temp_mon, t_temp_year);
			if (t_temp_date < 10)
			{
				dc->drawNumber(0, num_w_d, num_h);
				dc->drawNumber(t_temp_date, num_w_d + 14, num_h);
			}
			else
			{
				dc->drawNumber(t_temp_date, num_w_d, num_h);
			}
		}
		else if (pressed_button == up_mon_Button)
		{
			rusPrinter->print("     ", num_w_mon+10, num_h, TFT_BLACK, TFT_WHITE);
			t_temp_mon += 1;
			if (t_temp_mon == 13)
				t_temp_mon = 1;
			if (t_temp_mon < 10)
			{
				dc->drawNumber(0, num_w_mon, num_h);
				dc->drawNumber(t_temp_mon, num_w_mon + 14, num_h);
			}
			else
			{
				dc->drawNumber(t_temp_mon, num_w_mon, num_h);
			}
		}
		else if (pressed_button == up_year_Button)
		{
			rusPrinter->print("     ", num_w_y, num_h, TFT_BLACK, TFT_WHITE);
			t_temp_year += 1;
			if (t_temp_year > 99)
				t_temp_year = 99;
				dc->drawNumber(t_temp_year+2000, num_w_y-12, num_h);
		}
		else if (pressed_button == down_date_Button)
		{
			rusPrinter->print("    ", num_w_d+10, num_h, TFT_BLACK, TFT_WHITE);
			t_temp_date -= 1;
			t_temp_date = validateDate(t_temp_date, t_temp_mon, t_temp_year);
			if (t_temp_date < 10)
			{
				dc->drawNumber(0, num_w_d, num_h);
				dc->drawNumber(t_temp_date, num_w_d + 14, num_h);
			}
			else
			{
				dc->drawNumber(t_temp_date, num_w_d, num_h);
			}
		}
		else if (pressed_button == down_mon_Button)
		{
			rusPrinter->print("     ", num_w_mon+10, num_h, TFT_BLACK, TFT_WHITE);
			t_temp_mon -= 1;
			if (t_temp_mon == 0)
				t_temp_mon = 12;
			if (t_temp_mon < 10)
			{
				dc->drawNumber(0, num_w_mon, num_h);
				dc->drawNumber(t_temp_mon, num_w_mon + 14, num_h);
			}
			else
			{
				dc->drawNumber(t_temp_mon, num_w_mon, num_h);
			}
		}
		else if (pressed_button == down_year_Button)
		{
			rusPrinter->print("    ", num_w_y, num_h, TFT_BLACK, TFT_WHITE);
			t_temp_year -= 1;
			if (t_temp_year < 21)
				t_temp_year = 21;
			dc->drawNumber(t_temp_year+2000, num_w_y-12, num_h);
		}
	}
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTSetDateMenuScreen::draw(TFTMenu* menuManager)
 {
	 TFT_Class* dc = menuManager->getDC();

	 if (!dc)
	 {
		 return;
	 }


#ifdef USE_DS3231_REALTIME_CLOCK

	 TFTRus* rusPrinter = menuManager->getRusPrinter();
	 // setTextFont(TFT_FONT);
	 dc->setFreeFont(TFT_FONT);
	 dc->fillScreen(TFT_BLACK);
	 dc->setTextColor(TFT_WHITE);

	 rusPrinter->print("Установка даты", 25, 1, TFT_BLACK, TFT_WHITE);
	 rusPrinter->print("ДЕНЬ", 22, 162, TFT_BLACK, TFT_WHITE);
	 rusPrinter->print("МЕС.", 97, 162, TFT_BLACK, TFT_WHITE);
	 rusPrinter->print("ГОД", 170, 162, TFT_BLACK, TFT_WHITE);

	 RTC_DS3231  rtc = Settings.GetClock();
	 // DateTime dt = rtc.now();

	 RTCTime  dt = rtc.getTime();

	 int num_w = 33;
	 int num_h = 80;
	 // получаем текущую дату
	 t_temp_date = dt.dayOfMonth;
	 t_temp_mon = dt.month;
	 t_temp_year = 21;
	// t_temp_year = dt.year;
	 t_temp_dow = dt.dayOfWeek;
	 t_temp_hour = dt.hour;
	 t_temp_min = dt.minute;
	 t_temp_sec = dt.second;

	 if (t_temp_date == 0)
	 {
		 t_temp_date = 10;
		 t_temp_mon = 1;
		 t_temp_year = 21;
		 t_temp_dow = 2;
		 t_temp_hour = 9;
		 t_temp_min = 0;
		 t_temp_sec = 0;
	 }


	 if (t_temp_date < 10)
	 {
		 dc->drawNumber(0, num_w, num_h);
		 dc->drawNumber(t_temp_date, num_w + 14, num_h);
	 }
	 else
	 {
		 dc->drawNumber(t_temp_date, num_w, num_h);
	 }
	 if (t_temp_mon < 10)
	 {
		 num_w += 72;
		 dc->drawNumber(0, num_w, num_h);
		 dc->drawNumber(t_temp_mon, num_w + 14, num_h);
	 }
	 else
	 {
		 num_w += 72;
		 dc->drawNumber(t_temp_mon, num_w, num_h);
	 }
	
		 num_w += 60;
		 dc->drawNumber(t_temp_year+2000, num_w, num_h);
	

#endif 
	 if (screenButtons)
	 {
		 screenButtons->drawButtons(drawButtonsYield);
	 }
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------

 byte  TFTSetDateMenuScreen::validateDate(byte d, byte m, word y)
 {
	 byte mArr[12] = { 31,0,31,30,31,30,31,31,30,31,30,31 };
	 byte od;

	 if (m == 2)
	 {
		 if ((y % 4) == 0)
		 {
			 if (d == 30)
				 od = 1;
			 else if (d == 0)
				 od = 29;
			 else
				 od = d;
		 }
		 else
		 {
			 if (d == 29)
				 od = 1;
			 else if (d == 0)
				 od = 28;
			 else
				 od = d;
		 }
	 }
	 else
	 {
		 if (d == 0)
			 od = mArr[m - 1];
		 else if (d == (mArr[m - 1] + 1))
			 od = 1;
		 else
			 od = d;
	 }

	 return od;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 byte  TFTSetDateMenuScreen::validateDateForMonth(byte d, byte m, word y)
 {
	 byte mArr[12] = { 31,0,31,30,31,30,31,31,30,31,30,31 };
	 byte od;
	 //boolean dc = false;

	 if (m == 2)
	 {
		 if ((y % 4) == 0)
		 {
			 if (d > 29)
			 {
				 d = 29;
				 //dc = true;
			 }
		 }
		 else
		 {
			 if (d > 28)
			 {
				 d = 28;
				 //dc = true;
			 }
		 }
	 }
	 else
	 {
		 if (d > mArr[m - 1])
		 {
			 d = mArr[m - 1];
			 //dc = true;
		 }
	 }

	 return d;
 }


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TFTMenuScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

 TFTMenuScreen* MainScreen = NULL;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTMenuScreen::TFTMenuScreen()
 {
  tickerButton = -1;
  MainScreen = this;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 TFTMenuScreen::~TFTMenuScreen()
 {
	 delete screenButtons;	
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMenuScreen::onActivate(TFTMenu* menuManager)
 {
	 if (!menuManager->getDC())
	 {
		 return;
	 }
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMenuScreen::setup(TFTMenu* menuManager)
 {
	 TFT_Class* dc = menuManager->getDC();

	 if (!dc)
	 {
		 return;
	 }

	 screenButtons = new TFT_Buttons_Rus(dc, menuManager->getTouch(), menuManager->getRusPrinter());


	 screenButtons->setTextFont(TFT_FONT);
	 screenButtons->setSymbolFont(SENSOR_FONT);

	 screenButtons->setButtonColors(TFT_CHANNELS_BUTTON_COLORS);

	 int screenWidth = dc->width();
	 int screenHeight = dc->height();

   const int BUTTON_WIDTH = 220;
   const int BUTTON_HEIGHT = 40;
   const int BUTTON_HEIGHT1 = 40;
   const int V_SPACING = 10;
   const int V_SPACING1 = 60;

   int left = (screenWidth - BUTTON_WIDTH)/2;
   int top = (screenHeight - (BUTTON_HEIGHT * 4 + V_SPACING1)) / 2;
  
   top += BUTTON_HEIGHT*3 + V_SPACING;
   top += BUTTON_HEIGHT1 * 1 + V_SPACING;
   menuButton = screenButtons->addButton(left, top, BUTTON_WIDTH, BUTTON_HEIGHT1, MENU_CAPTION);     //Кнопка "СЕРВИСНОЕ МЕНЮ" на главном экране
   tmr = millis();

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  void TFTMenuScreen::onButtonPressed(TFTMenu* menuManager, int buttonID)
 {
	 tickerButton = -1;

	 //if (buttonID == dec25PercentsButton || buttonID == inc25PercentsButton || buttonID == dec50PercentsButton
		// || buttonID == inc50PercentsButton || buttonID == dec75PercentsButton || buttonID == inc75PercentsButton
		// || buttonID == dec100PercentsButton || buttonID == inc100PercentsButton)
	 //{
		// tickerButton = buttonID;
		// Ticker.start(this);
	 //}
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMenuScreen::onButtonReleased(TFTMenu* menuManager, int buttonID)
 {
	 Ticker.stop();
	 tickerButton = -1;
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMenuScreen::onTick()
 {
    TFTScreen->resetIdleTimer(); // сбрасываем таймер ничегонеделанья, чтобы не переключилось на главный экран
  
	/* if (tickerButton == dec25PercentsButton)
		 inc25Temp(-3);
	 else
		 if (tickerButton == inc25PercentsButton)
			 inc25Temp(3);
		 else
			 if (tickerButton == dec50PercentsButton)
				 inc50Temp(-3);
			 else
				 if (tickerButton == inc50PercentsButton)
					 inc50Temp(3);
				 else
					 if (tickerButton == dec75PercentsButton)
						 inc75Temp(-3);
					 else
						 if (tickerButton == inc75PercentsButton)
							 inc75Temp(3);
						 else
							 if (tickerButton == dec100PercentsButton)
								 inc100Temp(-3);
							 else
								 if (tickerButton == inc100PercentsButton)
									 inc100Temp(3);*/
 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  void TFTMenuScreen::update(TFTMenu* menuManager)
 {
    static uint32_t tmr = millis();
    if(millis() - tmr > DATA_MEASURE_THRESHOLD)
    {
        // drawVoltage(menuManager);

         // измерение закончено, просим датчик рассчитать значение 
        SensorData percents = Sensor.compute();

        if (Pa1ADCVal_tmp != percents.Value || Pa2ADCVal_tmp != percents.Fract)
        {
            Pa1ADCVal_tmp = percents.Value;
            Pa2ADCVal_tmp = percents.Fract;
            drawSensorPaData(menuManager);
        }


        drawDateTime(menuManager);
        drawWiFi(menuManager);
        tmr = millis();
    }

 
    int pressed_button = screenButtons->checkButtons(ButtonPressed, ButtonReleased);
    if(pressed_button != -1)
    {
        if (pressed_button == menuButton)
        {
            menuManager->switchToScreen("SERVICE_MENU"); //TOUCH_CALIBRATION SERVICE_MENU  REF_SCREEN
      
        }
    }

 }
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMenuScreen::draw(TFTMenu* menuManager)
 {
     TFT_Class* dc = menuManager->getDC();
     if (!dc)
     {
         return;
     }

     TFTRus* rusPrinter = menuManager->getRusPrinter();

	 if (screenButtons)
	 {
		 screenButtons->drawButtons(drawButtonsYield);
	 }

     drawDateTime(menuManager);
     drawWiFi(menuManager);

     String data = "ver.";
     data += Settings.getVer(); 

     int screenWidth = dc->width();
     int screenHeight = dc->height();

     dc->setFreeFont(TFT_SMALL_FONT);
     int textFontHeight = FONT_HEIGHT(dc);
     int textFontWidth = dc->textWidth(data);            // Returns pixel width of string in current font
     uint16_t curX = screenWidth - textFontWidth - 5;    // Координаты вывода 
     uint16_t curY = 2;                                  // Координаты вывода версии

     rusPrinter->print(data.c_str(), curX, curY, TFT_WHITE, TFT_BLACK); // Отображаем версию программы

    // curY = 18;    // Координаты вывода 
     dc->drawLine(2, 35, 317, 35, TFT_BLACK);
     dc->drawLine(2, 39, 317, 39, TFT_BLACK);

     drawSensorPaData(menuManager);
 }



 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 // Контроль внутреннего источника питания (аккумуляторов)
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMenuScreen::drawVoltage(TFTMenu* menuManager)
 {
	 TFT_Class* dc = menuManager->getDC();
	 if (!dc)
	 {
		 return;
	 }
	 TFTRus* rusPrinter = menuManager->getRusPrinter();


     //String data = "ver.";
     //data += Settings.getVer();

     //int screenWidth = dc->width();
     //int screenHeight = dc->height();

     //dc->setFreeFont(TFT_SMALL_FONT);
     //int textFontHeight = FONT_HEIGHT(dc);
     //int textFontWidth = dc->textWidth(data);            // Returns pixel width of string in current font
     //uint16_t curX = screenWidth - textFontWidth - 5;    // Координаты вывода 
     //uint16_t curY = 2;                                  // Координаты вывода версии

     //rusPrinter->print(data.c_str(), curX, curY, TFT_WHITE, TFT_BLACK); // Отображаем версию программы

	

	 dc->setFreeFont(TFT_FONT);

	 VoltageData vData5 = Settings.voltage5V;     // Контроль источника питания +5.0в

	 if (last5Vvoltage != vData5.raw)
	 {
		 last5Vvoltage = vData5.raw;
     
		 int y_val = 37;
		 int x_val = map(vData5.voltage5, 10, 230, 0, 100);

		 if (x_val < 20)
		 {
			 dc->fillRect(10, y_val, vData5.voltage5, 7, TFT_RED);
		 }
		 else if ((x_val >= 20) && (x_val < 60))
		 {
			 dc->fillRect(10, y_val, vData5.voltage5, 7, TFT_YELLOW);
		 }
		 else if (x_val >= 60)
		 {
			 dc->fillRect(10, y_val, vData5.voltage5, 7, TFT_GREEN);
		 }
		 dc->fillRect(vData5.voltage5, y_val-1, 230, 7+1, TFT_WHITE);

	 }

 }

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Отображение заряда источника питания (аккумуляторов)
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMenuScreen::chargeControl(TFTMenu* menuManager)
 {

	 TFT_Class* dc = menuManager->getDC();
	 if (!dc)
	 {
		 return;
	 }
	 //int y_val = 45;
	 int chargeControl_on = digitalRead(BATTERY_CHARGE);
	 if (chargeControl_on == 0 )
	 {
		 if (millis() - tmr > 30)
		 {
			 tmr = millis();
			 control_X++;
			 if (control_X > 220) control_X = 10;
			 dc->fillRect(control_X, y_val-1, 230, 7+1, TFT_WHITE);
			 dc->fillRect(10, y_val, control_X, 7, TFT_DARKBLUE);
			// dc->fillRect(control_X-10, y_val, 230, 7, TFT_WHITE);
			 charge_on = true;
		 }
	 }
	 else if(charge_on)
	 {
		 control_X = 10;
		 dc->fillRect(10, y_val, 230, 7, TFT_WHITE);
		 charge_on = false;
	 }

 }


 //======================================================
 void TFTMenuScreen::drawSensorPaData(TFTMenu* menuManager)
 {
     TFT_Class* dc = menuManager->getDC();
     TFTRus* rusPrinter = menuManager->getRusPrinter();

    // String data;

     SensorData percents = Sensor.compute();

     // стираем сообщение
    // drawMessage(menuManager, TFT_BACK_COLOR);

     // стираем время отсчёта
     //drawMeasureTime(menuManager, TFT_BACK_COLOR);

 /*    percents.Value = random(0,20);

     percents.Fract = random(0, 99);*/


     // выводим значение на экран
     String data;
     data += percents.Value;
     data += '.';
     if (percents.Fract < 10)
         data += '0';
     data += percents.Fract;
     data += " Pa  ";

     dc->setFreeFont(FF48);       // Select Free Serif 24 point font
     int screenWidth = dc->width();
     int screenHeight = dc->height();
     int fontHeight = FONT_HEIGHT(dc);

     int top = 90;

     int tw = rusPrinter->textWidth(data.c_str());

     int left = (screenWidth - tw) / 2;
     dc->fillRect(0, top - 15, 239, top + 15, TFT_BACK_COLOR);
     rusPrinter->print(data.c_str(), left, top, TFT_BACK_COLOR, TFT_BLACK);

 }


 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 // Вывод текущей даты и времени
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMenuScreen::drawDateTime(TFTMenu* menuManager)
 {

#ifdef USE_DS3231_REALTIME_CLOCK

	 TFT_Class* dc = menuManager->getDC();
	 if (!dc)
	 {
		 return;
	 }
	 TFTRus* rusPrinter = menuManager->getRusPrinter();

	 dc->setFreeFont(TFT_FONT);
	 //dc->setFreeFont(TFT_SMALL_FONT);

	 RTC_DS3231  rtc = Settings.GetClock();
	 // DateTime now = rtc.now();

	 RTCTime  now = rtc.getTime();  // обновить время

	 uint16_t curX = 5;     // Координаты вывода даты
	 uint16_t curY = 18;
	 //

	 // получаем компоненты даты в виде строк
	 String strDate = rtc.getDateStr(now);
	 String strTime = rtc.getTimeStr(now);

	 rusPrinter->print(strDate.c_str(), curX, curY, TFT_WHITE, TFT_BLACK);

	 dc->setFreeFont(TFT_FONT);

	 strTime += " ";

	 curX = 125;   // Координаты вывода времени
	 curY = 18;    // Координаты вывода 
	 rusPrinter->print(strTime.c_str(), curX, curY, TFT_WHITE, TFT_BLACK);

#endif 
 }


 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 // Вывод параметров WiFi
 //------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 void TFTMenuScreen::drawWiFi(TFTMenu* menuManager)
 {
     TFT_Class* dc = menuManager->getDC();
     if (!dc)
     {
         return;
     }
     TFTRus* rusPrinter = menuManager->getRusPrinter();

     dc->setFreeFont(TFT_FONT);
     //dc->setFreeFont(TFT_SMALL_FONT);
      uint16_t curX = 5;     // Координаты вывода WiFi
      uint16_t curY = 2;
      bool WiFi_On = Settings.GetWiFiState();
      bool WiFi_Connect = Settings.GetWiFiConnect();   // Признак подключения к роутеру

      WiFi_On = true;                 // Признак подключения модуля в работу
     // WiFi_Connect = true;            // Признак подключения к роутеру

      if (WiFi_On)
      {
          if (WiFi_Connect)
          {
              rusPrinter->print("          ", curX, curY, TFT_WHITE, TFT_WHITE);
              rusPrinter->print("WiFi on   ", curX, curY, TFT_WHITE, TFT_DARKGREEN);

          }
          else
          {
              rusPrinter->print("WiFi off", curX, curY, TFT_WHITE, TFT_RED);

          }
      }
      else
      {
          rusPrinter->print("          ", curX, curY, TFT_WHITE, TFT_WHITE);

      }


 }


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// KeyboardScreen
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
KeyboardScreen* Keyboard;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
KeyboardScreen::KeyboardScreen() : AbstractTFTScreen()
{
  inputTarget = NULL;
  maxLen = 20;
  isRusInput = true;
  
  if(!TFTScreen->getDC())
  {
    return;
  }
  

  buttons = new TFT_Buttons_Rus(TFTScreen->getDC(), TFTScreen->getTouch(),TFTScreen->getRusPrinter(),60);
  
  buttons->setTextFont(TFT_FONT);
  buttons->setButtonColors(TFT_CHANNELS_BUTTON_COLORS);
  buttons->setSymbolFont(VARIOUS_SYMBOLS_32x32);

  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
KeyboardScreen::~KeyboardScreen()
{
  for(size_t i=0;i<captions.size();i++)
  {
    delete captions[i];
  }
  for(size_t i=0;i<altCaptions.size();i++)
  {
    delete altCaptions[i];
  }
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyboardScreen::switchInput(bool redraw)
{
  isRusInput = !isRusInput;
  Vector<String*>* pVec = isRusInput ? &captions : &altCaptions;

  // у нас кнопки изменяемой клавиатуры начинаются с индекса 10
  size_t startIdx = 10;

  for(size_t i=startIdx;i<pVec->size();i++)
  {
    buttons->relabelButton(i,(*pVec)[i]->c_str(),redraw);
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyboardScreen::setup(TFTMenu* dc)
{
  if(!dc->getDC())
  {
    return;
  }
	
  createKeyboard(dc);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyboardScreen::update(TFTMenu* menu)
{
  if(!menu->getDC())
  {
    return;
  }
	
    // тут обновляем внутреннее состояние
    // раз нас вызвали, то пока не нажмут кнопки - мы не выйдем, поэтому всегда сообщаем, что на экране что-то происходит
    menu->resetIdleTimer();

    // мигаем курсором
    static uint32_t cursorTimer = millis();
    if(millis() - cursorTimer > 500)
    {
      static bool cursorVisible = true;
      cursorVisible = !cursorVisible;

      redrawCursor(menu,cursorVisible);

      cursorTimer = millis();
    }
    
    // проверяем на перемещение курсора
    TOUCH_Class* touch = menu->getTouch();

    uint16_t touch_x, touch_y;
    
    if(touch->getTouch(&touch_x, &touch_y))
    {
      // проверяем на попадание в прямоугольную область ввода текста
      TFT_Class* dc = menu->getDC();
      dc->setFreeFont(TFT_FONT);
      
      int screenWidth = dc->width();
      const int fontWidth = 8;
      
      if(touch_x >= KBD_SPACING && touch_x <= (screenWidth - KBD_SPACING) && touch_y >= KBD_SPACING && touch_y <= (KBD_SPACING + KBD_BUTTON_HEIGHT))
      {
        #ifdef USE_BUZZER
          Buzzer.buzz();
        #endif
        // кликнули на области ввода, ждём отпускания тача
        while (touch->getTouch(&touch_x, &touch_y)) { yield(); }
        

        // вычисляем, на какой символ приходится клик тачем
        int symbolNum = touch_x/fontWidth - 1;
        
        if(symbolNum < 0)
          symbolNum = 0;
          
        int valLen = menu->getRusPrinter()->getStringLength(inputVal.c_str());

        if(symbolNum > valLen)
          symbolNum = valLen;

        redrawCursor(menu,true);
        cursorPos = symbolNum;
        redrawCursor(menu,false);
      }
    } // if (touch->dataAvailable())
  
    int pressed_button = buttons->checkButtons(ButtonPressed, ButtonReleased);
    if(pressed_button != -1)
    {
      
       if(pressed_button == backspaceButton)
       {
        // удалить последний введённый символ
        drawValue(menu,true);
       }
       else
       if(pressed_button == okButton)
       {
          // закрыть всё нафик
          if(inputTarget)
          {
            inputTarget->onKeyboardInputResult(inputVal,true);
            inputVal = "";
          }
       }
        else
       if(pressed_button == switchButton)
       {
          // переключить раскладку
          switchInput(true);
       }
       else
       if(pressed_button == cancelButton)
       {
          // закрыть всё нафик
          if(inputTarget)
          {
            inputTarget->onKeyboardInputResult(inputVal,false);
            inputVal = "";
          }
       }
       else
       {
         // одна из кнопок клавиатуры, добавляем её текст к буферу, но - в позицию курсора!!!
         int oldLen = menu->getRusPrinter()->getStringLength(inputVal.c_str());
         const char* lbl = buttons->getLabel(pressed_button);
         
         if(!oldLen) // пустая строка
         {
          inputVal = lbl;
         }
         else
         if(oldLen < maxLen)
         {
            
            String buff;            
            const char* ptr = inputVal.c_str();
            
            for(int i=0;i<oldLen;i++)
            {
              unsigned char curChar = (unsigned char) *ptr;
              unsigned int charSz = utf8GetCharSize(curChar);
              for(byte k=0;k<charSz;k++) 
              {
                utf8Bytes[k] = *ptr++;
              }
              utf8Bytes[charSz] = '\0'; // добавляем завершающий 0
              
              if(i == cursorPos)
              {
                buff += lbl;
              }
              
              buff += utf8Bytes;
              
            } // for

            if(cursorPos >= oldLen)
              buff += lbl;

          inputVal = buff;
          
         } // if(oldLen < maxLen)
         

          int newLen = menu->getRusPrinter()->getStringLength(inputVal.c_str());

          if(newLen <= maxLen)
          {
            drawValue(menu);
                     
            if(newLen != oldLen)
            {
              redrawCursor(menu,true);
              cursorPos++;
              redrawCursor(menu,false);
            }
            
          }
          

         
       } // else одна из кнопок клавиатуры
    
    } // if(pressed_button != -1)
    
    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyboardScreen::draw(TFTMenu* menu)
{
  if(!menu->getDC())
  {
    return;
  }
	

  buttons->drawButtons(drawButtonsYield);

  TFT_Class* dc = menu->getDC();
  int screenWidth = dc->width();
  dc->drawRoundRect(KBD_SPACING, KBD_SPACING, screenWidth-KBD_SPACING*2, KBD_BUTTON_HEIGHT,2, TFT_LIGHTGREY);

  drawValue(menu);
  redrawCursor(menu,false);
}
//--------------------------------------------------------------------------------------------------------------------------------
void KeyboardScreen::redrawCursor(TFTMenu* menu, bool erase)
{
  TFT_Class* dc = menu->getDC();
  TFTRus* rus = menu->getRusPrinter();

  dc->setFreeFont(TFT_FONT);
  uint8_t fontHeight = FONT_HEIGHT(dc);

  int top = KBD_SPACING + (KBD_BUTTON_HEIGHT - fontHeight)/2;
  
  String tmp = inputVal.substring(0,cursorPos);
  
  int left = KBD_SPACING*2 + rus->textWidth(tmp.c_str());

  uint16_t fgColor = TFT_BACK_COLOR;

  if(erase)
    fgColor = TFT_BACK_COLOR;
  else
    fgColor = TFT_FONT_COLOR;
  
  dc->fillRect(left,top,1,fontHeight,fgColor);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyboardScreen::drawValue(TFTMenu* menu, bool deleteCharAtCursor)
{
  if(!inputVal.length())
    return;


   TFT_Class* dc = menu->getDC();

  if(deleteCharAtCursor)
  {
    // надо удалить символ слева от позиции курсора.

    String buff;
    int len = menu->getRusPrinter()->getStringLength(inputVal.c_str());
    const char* ptr = inputVal.c_str();
    
    for(int i=0;i<len;i++)
    {
      unsigned char curChar = (unsigned char) *ptr;
      unsigned int charSz = utf8GetCharSize(curChar);
      for(byte k=0;k<charSz;k++) 
      {
        utf8Bytes[k] = *ptr++;
      }
      utf8Bytes[charSz] = '\0'; // добавляем завершающий 0
      
      if(i != (cursorPos-1)) // игнорируем удаляемый символ
      {
        buff += utf8Bytes;
      }
      
    } // for
    
    buff += ' '; // маскируем последний символ для корректной перерисовки на экране
    inputVal = buff;

  }

  dc->setFreeFont(TFT_FONT);
  
  uint8_t fontHeight = FONT_HEIGHT(dc);


  int top = KBD_SPACING + (KBD_BUTTON_HEIGHT - fontHeight)/2;
  int left = KBD_SPACING*2;

  menu->getRusPrinter()->print(inputVal.c_str(),left,top,TFT_BACK_COLOR,TFT_FONT_COLOR);

  if(deleteCharAtCursor)
  {
    // если надо удалить символ слева от позиции курсора, то в этом случае у нас последний символ - пробел, и мы его удаляем
    inputVal.remove(inputVal.length()-1,1);

    redrawCursor(menu,true);

    cursorPos--;
    if(cursorPos < 0)
      cursorPos = 0;

    redrawCursor(menu,false);
  }
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyboardScreen::createKeyboard(TFTMenu* menu)
{
  buttons->deleteAllButtons();

  TFT_Class* dc = menu->getDC();
  int screenWidth = dc->width();
  int screenHeight = dc->height();  

  // создаём клавиатуру

  int colCounter = 0;
  int left = KBD_SPACING;
  int top = KBD_SPACING*2 + KBD_BUTTON_HEIGHT;

  // сперва у нас кнопки 0-9
  for(uint8_t i=0;i<10;i++)
  {
    char c = '0' + i;
    String* s = new String(c);
    captions.push_back(s);

    String* altS = new String(c);
    altCaptions.push_back(altS);    

    /*int addedBtn = */buttons->addButton(left, top, KBD_BUTTON_WIDTH, KBD_BUTTON_HEIGHT, s->c_str());
   // buttons->setButtonBackColor(addedBtn, VGA_GRAY);
   // buttons->setButtonFontColor(addedBtn, VGA_BLACK);
    
    left += KBD_BUTTON_WIDTH + KBD_SPACING;
    colCounter++;
    if(colCounter >= KBD_BUTTONS_IN_ROW)
    {
      colCounter = 0;
      left = KBD_SPACING;
      top += KBD_SPACING + KBD_BUTTON_HEIGHT;
    }
  }
  // затем - А-Я
  const char* letters[] = {
    "А", "Б", "В", "Г", "Д", "Е",
    "Ж", "З", "И", "Й", "К", "Л",
    "М", "Н", "О", "П", "Р", "С",
    "Т", "У", "Ф", "Х", "Ц", "Ч",
    "Ш", "Щ", "Ъ", "Ы", "Ь", "Э",
    "Ю", "Я", NULL
  };

  const char* altLetters[] = {
    "A", "B", "C", "D", "E", "F",
    "G", "H", "I", "J", "K", "L",
    "M", "N", "O", "P", "Q", "R",
    "S", "T", "U", "V", "W", "X",
    "Y", "Z", ".", ",", ":", ";",
    "!", "?", NULL
  };  

  int lettersIterator = 0;
  while(letters[lettersIterator])
  {
    String* s = new String(letters[lettersIterator]);
    captions.push_back(s);

    String* altS = new String(altLetters[lettersIterator]);
    altCaptions.push_back(altS);

    buttons->addButton(left, top, KBD_BUTTON_WIDTH, KBD_BUTTON_HEIGHT, s->c_str());
    left += KBD_BUTTON_WIDTH + KBD_SPACING;
    colCounter++;
    if(colCounter >= KBD_BUTTONS_IN_ROW)
    {
      colCounter = 0;
      left = KBD_SPACING;
      top += KBD_SPACING + KBD_BUTTON_HEIGHT;
    } 

    lettersIterator++;
  }
  // затем - кнопка переключения ввода
    switchButton = buttons->addButton(left, top, KBD_BUTTON_WIDTH, KBD_BUTTON_HEIGHT, "q", BUTTON_SYMBOL);
    buttons->setButtonBackColor(switchButton, TFT_MAROON);
    buttons->setButtonFontColor(switchButton, TFT_WHITE);

    left += KBD_BUTTON_WIDTH + KBD_SPACING;
  
  // затем - пробел,
    spaceButton = buttons->addButton(left, top, KBD_BUTTON_WIDTH*5 + KBD_SPACING*4, KBD_BUTTON_HEIGHT, " ");
    //buttons->setButtonBackColor(spaceButton, VGA_GRAY);
    //buttons->setButtonFontColor(spaceButton, VGA_BLACK);
       
    left += KBD_BUTTON_WIDTH*5 + KBD_SPACING*5;
   
  // backspace, 
    backspaceButton = buttons->addButton(left, top, KBD_BUTTON_WIDTH*2 + KBD_SPACING, KBD_BUTTON_HEIGHT, ":", BUTTON_SYMBOL);
    buttons->setButtonBackColor(backspaceButton, TFT_MAROON);
    buttons->setButtonFontColor(backspaceButton, TFT_WHITE);

    left = KBD_SPACING;
    top = screenHeight - KDB_BIG_BUTTON_HEIGHT - KBD_SPACING;
   
  // OK,
    int okCancelButtonWidth = (screenWidth - KBD_SPACING*3)/2;
    okButton = buttons->addButton(left, top, okCancelButtonWidth, KDB_BIG_BUTTON_HEIGHT, "OK");
    left += okCancelButtonWidth + KBD_SPACING;
  
  // CANCEL
    cancelButton = buttons->addButton(left, top, okCancelButtonWidth, KDB_BIG_BUTTON_HEIGHT, "ОТМЕНА");

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyboardScreen::applyType(KeyboardType keyboardType)
{
  if(ktFull == keyboardType)
  {
    buttons->enableButton(spaceButton);
    buttons->enableButton(switchButton);

    // включаем все кнопки
    // у нас кнопки изменяемой клавиатуры начинаются с индекса 10
    size_t startIdx = 10;
  
    for(size_t i=startIdx;i<altCaptions.size();i++)
    {
      buttons->enableButton(i);
    }    

    isRusInput = false;
    switchInput(false);

    return;
  }

  if(ktNumbers == keyboardType)
  {
    buttons->disableButton(spaceButton);
    buttons->disableButton(switchButton);

    // выключаем все кнопки, кроме номеров и точки
    // у нас кнопки изменяемой клавиатуры начинаются с индекса 10
    size_t startIdx = 10;
  
    for(size_t i=startIdx;i<altCaptions.size();i++)
    {
      if(*(altCaptions[i]) != ".")
        buttons->disableButton(i);
    }        

    isRusInput = true;
    switchInput(false);

    return;
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void KeyboardScreen::show(const String& val, int ml, KeyboardInputTarget* it, KeyboardType keyboardType, bool eng)
{
  if(!TFTScreen->getDC())
  {
    return;
  }
	
  inputVal = val;
  inputTarget = it;
  maxLen = ml;

  cursorPos = TFTScreen->getRusPrinter()->getStringLength(inputVal.c_str());

  applyType(keyboardType);

  if(eng && isRusInput)
    switchInput(false);

  TFTScreen->switchToScreen(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractTFTScreen* KeyboardScreen::create()
{
    Keyboard = new KeyboardScreen();
    return Keyboard;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageBoxScreen* MessageBox;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
MessageBoxScreen::MessageBoxScreen() : AbstractTFTScreen()
{
  targetOkScreen = NULL;
  targetCancelScreen = NULL;
  resultSubscriber = NULL;
  caption = NULL;
  
  if(!TFTScreen->getDC())
  {
    return;
  }
  

  buttons = new TFT_Buttons_Rus(TFTScreen->getDC(), TFTScreen->getTouch(),TFTScreen->getRusPrinter());
  
  buttons->setTextFont(TFT_FONT);
  buttons->setButtonColors(TFT_CHANNELS_BUTTON_COLORS);
   
  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::setup(TFTMenu* dc)
{
  if(!dc->getDC())
  {
    return;
  }

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::update(TFTMenu* dc)
{
  if(!dc->getDC())
  {
    return;
  }
	
    // тут обновляем внутреннее состояние    
 
    int pressed_button = buttons->checkButtons(ButtonPressed, ButtonReleased);
    if(pressed_button != -1)
    {
      // сообщаем, что у нас нажата кнопка
      dc->resetIdleTimer();
      
       if(pressed_button == noButton && targetCancelScreen)
       {
        if(resultSubscriber)
          resultSubscriber->onMessageBoxResult(false);
          
        dc->switchToScreen(targetCancelScreen);
       }
       else
       if(pressed_button == yesButton && targetOkScreen)
       {
          if(resultSubscriber)
            resultSubscriber->onMessageBoxResult(true);
            
            dc->switchToScreen(targetOkScreen);
       }
    
    } // if(pressed_button != -1)

    
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::draw(TFTMenu* hal)
{
  TFT_Class* dc = hal->getDC();
  
  if(!dc)
  {
    return;
  }
  
  dc->setFreeFont(TFT_FONT);
  TFTRus* rusPrinter = hal->getRusPrinter();
  
  uint8_t fontHeight = FONT_HEIGHT(dc);
  
  int displayWidth = dc->width();
  int displayHeight = dc->height();
  
  int lineSpacing = 6; 
  int topOffset = 10;
  int curX = 0;
  int curY = topOffset;

  int lineLength = 0;

  uint16_t fgColor = TFT_NAVY, bgColor = TFT_WHITE;
  
  // подложка под заголовок
  if(boxType == mbHalt && errorColors)
  {
    fgColor = TFT_RED;
  }
  else
  {
    fgColor = TFT_NAVY;
  }
    
  dc->fillRect(0, 0, displayWidth, topOffset + fontHeight+4,fgColor);
  
  if(caption)
  {
    if(boxType == mbHalt && errorColors)
    {
      bgColor = TFT_RED;
      fgColor = TFT_WHITE;
    }
    else
    {
      bgColor = TFT_NAVY;
      fgColor = TFT_WHITE;      
    }
    lineLength = rusPrinter->textWidth(caption);
    curX = (displayWidth - lineLength)/2; 
    rusPrinter->print(caption,curX,curY,bgColor,fgColor);
  }

  curY = (displayHeight - ALL_CHANNELS_BUTTON_HEIGHT - (lines.size()*fontHeight + (lines.size()-1)*lineSpacing))/2;

  for(size_t i=0;i<lines.size();i++)
  {
    lineLength = rusPrinter->textWidth(lines[i]);
    curX = (displayWidth - lineLength)/2;    
    rusPrinter->print(lines[i],curX,curY,TFT_BACK_COLOR,TFT_FONT_COLOR);
    curY += fontHeight + lineSpacing;
  }

  buttons->drawButtons(drawButtonsYield);

  if(boxType == mbHalt && haltInWhile)
  {
    while(1)
    {
      #ifdef USE_EXTERNAL_WATCHDOG
        updateExternalWatchdog();
      #endif      
    }
  }

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::recreateButtons()
{
  buttons->deleteAllButtons();
  yesButton = -1;
  noButton = -1;
  
  TFT_Class* dc = TFTScreen->getDC();
  
  int screenWidth = dc->width();
  int screenHeight = dc->height();
  int buttonsWidth = 200;

  int numOfButtons = boxType == mbShow ? 1 : 2;

  int top = screenHeight - ALL_CHANNELS_BUTTON_HEIGHT - INFO_BOX_V_SPACING;
  int left = (screenWidth - (buttonsWidth*numOfButtons + INFO_BOX_V_SPACING*(numOfButtons-1)))/2;
  
  yesButton = buttons->addButton(left, top, buttonsWidth, ALL_CHANNELS_BUTTON_HEIGHT, boxType == mbShow ? "OK" : "ДА");

  if(boxType == mbConfirm)
  {
    left += buttonsWidth + INFO_BOX_V_SPACING;
    noButton = buttons->addButton(left, top, buttonsWidth, ALL_CHANNELS_BUTTON_HEIGHT, "НЕТ");  
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::halt(const char* _caption, Vector<const char*>& _lines, bool _errorColors, bool _haltInWhile)
{
  if(!TFTScreen->getDC())
  {
    return;
  }
	
  lines = _lines;
  caption = _caption;
  boxType = mbHalt;
  errorColors = _errorColors;
  haltInWhile = _haltInWhile;

  buttons->deleteAllButtons();
  yesButton = -1;
  noButton = -1;
    
  targetOkScreen = NULL;
  targetCancelScreen = NULL;
  resultSubscriber = NULL;  

  TFTScreen->switchToScreen(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::show(const char* _caption, Vector<const char*>& _lines, AbstractTFTScreen* okTarget, MessageBoxResultSubscriber* sub)
{
  if(!TFTScreen->getDC())
  {
    return;
  }
	
  lines = _lines;
  caption = _caption;
  errorColors = false;

  boxType = mbShow;
  recreateButtons();
    
  targetOkScreen = okTarget;
  targetCancelScreen = NULL;
  resultSubscriber = sub;

  TFTScreen->switchToScreen(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MessageBoxScreen::confirm(const char* _caption, Vector<const char*>& _lines, AbstractTFTScreen* okTarget, AbstractTFTScreen* cancelTarget, MessageBoxResultSubscriber* sub)
{
  if(!TFTScreen->getDC())
  {
    return;
  }
	
  lines = _lines;
  caption = _caption;
  errorColors = false;

  boxType = mbConfirm;
  recreateButtons();
  
  targetOkScreen = okTarget;
  targetCancelScreen = cancelTarget;
  resultSubscriber = sub;

  TFTScreen->switchToScreen(this);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AbstractTFTScreen* MessageBoxScreen::create()
{
    MessageBox = new MessageBoxScreen();
    return MessageBox;  
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// TickerClass
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TickerClass Ticker;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TickerClass::TickerClass()
{
  started = false;
  beforeStartTickInterval = 1000;
  tickInterval = 100;
  waitBefore = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TickerClass::~TickerClass()
{
  stop();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TickerClass::setIntervals(uint16_t _beforeStartTickInterval,uint16_t _tickInterval)
{
  beforeStartTickInterval = _beforeStartTickInterval;
  tickInterval = _tickInterval;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TickerClass::start(ITickHandler* h)
{
  if(started)
    return;

  handler = h;

  timer = millis();
  waitBefore = true;
  started = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TickerClass::stop()
{
  if(!started)
    return;

  handler = NULL;

  started = false;
  waitBefore = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TickerClass::tick()
{
  if(!started)
    return;

  uint32_t now = millis();

  if(waitBefore)
  {
    if(now - timer > beforeStartTickInterval)
    {
      waitBefore = false;
      timer = now;
      if(handler)
        handler->onTick();
    }
    return;
  }

  if(now - timer > tickInterval)
  {
    timer = now;
    if(handler)
      handler->onTick();
  }

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif // USE_TFT_MODULE
