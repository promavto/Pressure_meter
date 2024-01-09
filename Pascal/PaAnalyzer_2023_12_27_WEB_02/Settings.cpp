#include "Settings.h"
#include "TFTMenu.h"
#include <Wire.h>                 //
#include "Configuration_ESP32.h"  // Основные настройки программы
#include "AT24CX.h"
//#include "EEPROM.h"
#include "Memory.h"

//#define EEPROM_SIZE 500
// EEPROM object
AT24CX mem;

#define FONT_HEIGHT(dc) dc->fontHeight(1)


//--------------------------------------------------------------------------------------------------------------------------------
SettingsClass Settings;
//--------------------------------------------------------------------------------------------------------------------------------
SettingsClass::SettingsClass()
{
	voltage3V.raw      = 0;
    voltage5V.raw      = 0;
	voltage3V.voltage3 = 0;
    voltage5V.voltage5 = 0;

}



//--------------------------------------------------------------------------------------------------------------------------------

//void SettingsClass::Set0_O2Value(uint16_t val)
//{
//    mem.write(S0_O2Value_ADDRESS, CORRECT_DATA);
//    mem.writeInt(S0_O2Value_ADDRESS + 2, val);
//}
////--------------------------------------------------------------------------------------------------------------------------------
//uint16_t SettingsClass::Get0_O2Value()
//{
//	byte corr_data = mem.read(S0_O2Value_ADDRESS);
//    uint16_t result = mem.readInt(S0_O2Value_ADDRESS + 2);
//    if (corr_data != CORRECT_DATA)
//    {
//        result = 0;    // нет калибровочных коэффициентов для 0% О2
//    }
//
//    return result;  
//}
////--------------------------------------------------------------------------------------------------------------------------------
//void SettingsClass::Set20_9_O2Value(uint16_t val)
//{
//    mem.write(S20_9_O2Value_ADDRESS, CORRECT_DATA);
//    mem.writeInt(S20_9_O2Value_ADDRESS + 2, val);
//}
////--------------------------------------------------------------------------------------------------------------------------------
//uint16_t SettingsClass::Get20_9_O2Value()
//{
//	byte corr_data = mem.read(S20_9_O2Value_ADDRESS);
//    uint16_t result = mem.readInt(S20_9_O2Value_ADDRESS + 2);
//    if (corr_data != CORRECT_DATA)
//    {
//        result = 0;     // нет калибровочных коэффициентов для 20,9% О2
//    }
//
//    return result;
//}
//--------------------------------------------------------------------------------------------------------------------------------
uint16_t SettingsClass::GetCalibrationTime()
{
	byte corr_data = mem.read(CalibrationTime_ADDRESS);
    uint16_t result = mem.readInt(CalibrationTime_ADDRESS + 2);
    if (corr_data != CORRECT_DATA)
    {
        result = DEFAULT_CALIBRATION_TIME; // по умолчанию 60 секунд на калибровку
    }

    if(result < 1)
    {
        result = DEFAULT_CALIBRATION_TIME;
    }

    return result;  
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::SetCalibrationTime(uint16_t val)
{
  if(val < 1)
  {
    val = DEFAULT_CALIBRATION_TIME;
  }

  mem.write(CalibrationTime_ADDRESS, CORRECT_DATA);
  mem.writeInt(CalibrationTime_ADDRESS + 2, val);
}
//--------------------------------------------------------------------------------------------------------------------------------
uint16_t SettingsClass::GetMeasureTime()
{
	byte corr_data = mem.read(MeasureTime_ADDRESS);
    uint16_t result    = mem.readInt(MeasureTime_ADDRESS+1);
  if(corr_data != CORRECT_DATA)
  {
    result = DEFAULT_MEASURE_TIME;    // по умолчанию 30 секунд на изменение
  }

  if(result < 1)
  {
    result = DEFAULT_MEASURE_TIME;
  }

  return result;
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::SetMeasureTime(uint16_t val)
{
  if(val < 1)
  {
    val = DEFAULT_MEASURE_TIME;
  }
  mem.write(MeasureTime_ADDRESS, CORRECT_DATA);
  mem.writeInt(MeasureTime_ADDRESS+1, val);
}
//--------------------------------------------------------------------------------------------------------------------------------
TFTCalibrationData SettingsClass::GetTftCalibrationData()
{
  TFTCalibrationData result;

  byte corr_data = mem.read(TftCalibrationData_ADDRESS);
  result.isValid = mem.read(TftCalibrationData_ADDRESS+2);
  int adr_data = 0;

  for (int i = 0; i < 5; i++)
  {
      result.points[i] = mem.readInt(TftCalibrationData_ADDRESS + 4 + adr_data);
      adr_data++;
      adr_data++;
  }

  if (corr_data != CORRECT_DATA)
  {
    result.isValid = false;
    return result;
  }
  
  result.isValid = true;
  return result;  
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::SetTftCalibrationData(TFTCalibrationData& data)
{
  data.isValid = true;
 
  mem.write(TftCalibrationData_ADDRESS, CORRECT_DATA);
  mem.write(TftCalibrationData_ADDRESS+2, data.isValid);

  int adr_data = 0;

  for (int i = 0; i < 5; i++)
  {
      mem.writeInt(TftCalibrationData_ADDRESS + 4+ adr_data, data.points[i]);
      adr_data++;
      adr_data++;
  }
  
}

//--------------------------------------------------------------------------------------------------------------------------------
uint16_t SettingsClass::GetReference_voltage()
{
	byte corr_dataR = mem.read(Reference_voltage_ADDRESS);          // Считываем признак корректности данных
	
    uint16_t result = mem.readInt(Reference_voltage_ADDRESS + 1);   // Считываем уровень опорного напряжения из памяти
    //if (corr_dataR != CORRECT_DATA)                               // Если данные не корректные
    //{
    //    result = DEFAULT_Reference_voltage;                       // Устанавливаем данные по умолчанию 
    //}
	Serial.print("corr_data ... ");
	Serial.println(corr_dataR);
	Serial.print(corr_dataR);
	Serial.print("/data ");
	Serial.println(result);
    //if (result < 10)                                               // Если данные меньше 1
    //{
    //    result = DEFAULT_Reference_voltage;                       // Устанавливаем данные по умолчанию 
    //}

    return result;
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::SetReference_voltage(uint16_t val)
{
 //   if (val > 1000)                                                 // Если данные больше
 //   {
 //       val = DEFAULT_Reference_voltage;                            // Устанавливаем данные по умолчанию 
	//}
	Serial.print("/SetReference.. ");
	Serial.print(val);
    mem.write(Reference_voltage_ADDRESS, CORRECT_DATA);          // Сохраняем признак корректности данных
	delay(50);
    mem.writeInt(Reference_voltage_ADDRESS + 1, val);               // Сохраняем измеренные данные опорного напряжения
	delay(50);
	Serial.print("  Set corr_data ... ");
	Serial.print(CORRECT_DATA);

	byte corr_dataR = mem.read(Reference_voltage_ADDRESS);     // Считываем признак корректности данных
	delay(50);
	Serial.print("/read ");
	Serial.print(corr_dataR);
	uint16_t result = mem.readInt(Reference_voltage_ADDRESS + 1); // Считываем уровень опорного напряжения из памяти
	Serial.print("/data ");
	Serial.println(result);
}
//--------------------------------------------------------------------------------------------------------------------------------

uint16_t SettingsClass::GetTimeLedLCD()  // Получить время отключения подсветки дисплея при отсутсвии активности
{
	byte corr_data = mem.read(TimeLedLCD_ADDRESS);
    uint16_t result = mem.readInt(TimeLedLCD_ADDRESS + 2);
    if (corr_data != CORRECT_DATA)
    {
        result = BACKLIGHT_OFF_DELAY; // по умолчанию 
    }

    if (result < 1)
    {
        result = BACKLIGHT_OFF_DELAY;
    }

    return result;
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::SetTimeLedLCD(uint16_t val) // Записать время отключения подсветки дисплея при отсутсвии активности
{
    if (val < 1)
    {
        val = BACKLIGHT_OFF_DELAY;
    }
    mem.write(TimeLedLCD_ADDRESS, CORRECT_DATA);
    mem.writeInt(TimeLedLCD_ADDRESS + 2, val);
}
//--------------------------------------------------------------------------------------------------------------------------------

uint16_t SettingsClass::GetTimePowerOff()  // Получить время отключения прибора при отсутсвии активности
{
	byte corr_data = mem.read(TimePowerOff_ADDRESS);
    uint16_t result = mem.readInt(TimePowerOff_ADDRESS + 2);

    if (corr_data != CORRECT_DATA)
    {
        result = TIME_POWER_OFF_DELAY; // по умолчанию 
    }

    if (result < 1)
    {
        result = TIME_POWER_OFF_DELAY;
    }

    return result;
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::SetTimePowerOff(uint16_t val) // Записать время отключения прибора при отсутсвии активности
{
    if (val < 1)
    {
        val = TIME_POWER_OFF_DELAY;
    }
    mem.write(TimePowerOff_ADDRESS, CORRECT_DATA);
    mem.writeInt(TimePowerOff_ADDRESS + 2, val);
}
//--------------------------------------------------------------------------------------------------------------------------------

void SettingsClass::turnPowerOff()
{
     // выключаем питание контроллера
    digitalWrite(POWER_ON_OUT, LOW);
}
//--------------------------------------------------------------------------------------------------------------------------------
PowerType SettingsClass::getPowerType()
{
    return powerType;
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::checkPower() // Определение от какого источника питается прибор. Определяется в момент включения прибора.
{
    if (!digitalRead(POWER_ON_IN))
    {
        Settings.powerType = batteryPower;
		DBGLN("");
        DBGLN(F("BATTERY POWER !!!"));
    }
    else
    {
        Settings.powerType = powerViaUSB;
		DBGLN("");
        DBGLN(F("POWER  VIA USB !!!"));
    }
}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::displayBacklight(bool bOn)
{
    digitalWrite(LCD_LED, bOn ? LOW : HIGH);
    backlightFlag = bOn;
}
//--------------------------------------------------------------------------------------------------------------------------------
uint16_t SettingsClass::getPowerVoltage5(uint16_t pin) // Контроль напряжения питания внутренних источников (аккумуляторов).
{
    for (int i = 0; i < 20; i++)
    {
        voltage5V.raw += analogRead(pin);
    }
    voltage5V.raw = voltage5V.raw/20;

	dimension_array[array_count] = voltage5V.raw;
	array_count++;
	int val_voltage = 0;
	if (array_count > array_size)                    // проверка заполнения массива первичными данными о уровне напряжения аккумулятора
	{
		array_count = 0;
		array_countMax = true;                        //Разрешить выдавать данные об уровне напряжения аккумулятора
	}

	sum = 0;                                         //

	if (array_countMax)                              // формируем данные об уровне напряжения аккумулятора
	{
		for (int i = 0; i < array_size; i++)
		{
			sum += dimension_array[i];
		}
		val_voltage = sum / array_size;
	}
	else
	{
		for (int i = 0; i < array_count; i++)       //формируем первичные (заполняем массив) данные об уровне напряжения аккумулятора
		{
			sum += dimension_array[array_count - 1];
		}
		val_voltage = sum / array_count;
	}
	// Serial.print("voltage Bat - ");
	//Serial.println(val_voltage);

	voltage5V.voltage5 = map(val_voltage, 1060, 2150, 10, 230);
	if (voltage5V.voltage5 > 230) voltage5V.voltage5 = 230;  // Напряжение питания  

    return voltage5V.voltage5;
}

//--------------------------------------------------------------------------------------------------------------------------------
bool SettingsClass::getTouch() // Чтение нажатия на экран
{
    bool touch = digitalRead(TOUCH_IRQ);
    return touch;
}
//--------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_DS3231_REALTIME_CLOCK
RTC_DS3231& SettingsClass::GetClock()
{
    return _rtc;
}
#endif

//--------------------------------------------------------------------------------------------------------------------------------------
uint8_t SettingsClass::read8(uint16_t address, uint8_t defaultVal)
{
   
    uint8_t curVal = mem.read(address);
    if (curVal == 0xFF)
        curVal = defaultVal;

    return curVal;
}

//--------------------------------------------------------------------------------------------------------------------------------------
uint16_t SettingsClass::read16(uint16_t address, uint16_t defaultVal)
{
    uint16_t val = 0;
    byte* b = (byte*)&val;

    for (byte i = 0; i < 2; i++)
        *b++ = mem.read(address + i);

    if (val == 0xFFFF)
        val = defaultVal;

    return val;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::write16(uint16_t address, uint16_t val)
{
    byte* b = (byte*)&val;

    for (byte i = 0; i < 2; i++)
        mem.write(address + i, *b++);

}
//--------------------------------------------------------------------------------------------------------------------------------------
unsigned long SettingsClass::read32(uint16_t address, unsigned long defaultVal)
{
    unsigned long val = 0;
    byte* b = (byte*)&val;

    for (byte i = 0; i < 4; i++)
        *b++ = mem.read(address + i);

    if (val == 0xFFFFFFFF)
        val = defaultVal;

    return val;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::write32(uint16_t address, unsigned long val)
{
    byte* b = (byte*)&val;

    for (byte i = 0; i < 4; i++)
        mem.write(address + i, *b++);
}
//--------------------------------------------------------------------------------------------------------------------------------------
String SettingsClass::readString(uint16_t address, byte maxlength)
{
    String result;
    Serial.println("readString ..");
    for (byte i = 0; i < maxlength; i++)
    {
        byte b = mem.read(address++);
        if (b == 0/*'\0'*/)
        {
            Serial.print("break ..");
            Serial.println((char)b, HEX);
            Serial.print("address ..");
            Serial.println(address);
            break;
        }
 
        result += (char)b;
 		Serial.print((char)b);

    }
 
	//Serial.println(result);
    Serial.println();
    return result;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::writeString(uint16_t address, const String& v, byte maxlength)
{
    byte val = v.length();
    Serial.println("writeString ..");
    for (byte i = 0; i < maxlength; i++)
    {
        if (i >= v.length())
            break;

		mem.write(address++, v[i]);
		Serial.print(v[i]);
    }
    // пишем завершающий ноль
	mem.write(address + v.length()+1, 0/*'\0'*/);
 /*   Serial.print("address ..");
    Serial.println(address);*/
    Serial.println();
}

//--------------------------------------------------------------------------------------------------------------------------------------
bool SettingsClass::GetWiFiState()
{
    wifiState = mem.read(WIFI_STATE_EEPROM_ADDR);
    return wifiState;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::SetWiFiState(bool st)
{
    wifiState = st;
	mem.write(WIFI_STATE_EEPROM_ADDR, st);
}

//--------------------------------------------------------------------------------------------------------------------------------------
bool SettingsClass::GetWiFiConnect()
{
    return wifiConnect;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::SetWiFiConnect(bool WiFiOn)
{
    wifiConnect = WiFiOn;
}


//--------------------------------------------------------------------------------------------------------------------------------------
String SettingsClass::GetStationPassword()
{
 
	byte v_length = byte(mem.read(STATION_PASSWORD_EEPROM_ADDR + 1));
	stationPassword = readString(STATION_PASSWORD_EEPROM_ADDR + 2, 20/*v_length*/);
    return stationPassword;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::SetStationPassword(const String& v)
{
   // stationPassword = v;

	mem.write(STATION_PASSWORD_EEPROM_ADDR, CORRECT_DATA);
	mem.write(STATION_PASSWORD_EEPROM_ADDR + 1, v.length());
	writeString(STATION_PASSWORD_EEPROM_ADDR + 2, v, 20/*v_length*/);
}
//--------------------------------------------------------------------------------------------------------------------------------------
String SettingsClass::GetStationID()
{

	byte v_length = byte(mem.read(STATION_ID_EEPROM_ADDR + 1));
	stationID = readString(STATION_ID_EEPROM_ADDR + 2, 20/*v_length*/);

    return stationID;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::SetStationID(const String& v)
{

	mem.write(STATION_ID_EEPROM_ADDR, CORRECT_DATA);
	mem.write(STATION_ID_EEPROM_ADDR+1, v.length());
    writeString(STATION_ID_EEPROM_ADDR + 2, v, 20/*v_length*/);
}
//--------------------------------------------------------------------------------------------------------------------------------------

String SettingsClass::GetRouterID()
{/*
    char saved[MAX_WIFI_LENGTH + 1] = { 0 };

    byte corr_data = mem.read(ROUTER_ID_EEPROM_ADDR);
    mem.readChars(ROUTER_ID_EEPROM_ADDR + 1, saved, sizeof(saved));

    if (corr_data != CORRECT_DATA)
    {
        return MENU_PASSWORD;
    }

    saved[MAX_WIFI_LENGTH] = '\0';
    return saved;*/





	////byte v_length = byte(mem.read(ROUTER_ID_EEPROM_ADDR + 1));
	routerID = readString(ROUTER_ID_EEPROM_ADDR + 2, 20/*v_length*/);
    return routerID;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::SetRouterID(const String& v)
{
    //routerID = v;
    byte len = v.length();
	/*mem.write(ROUTER_ID_EEPROM_ADDR, CORRECT_DATA);
	mem.write(ROUTER_ID_EEPROM_ADDR + 1, len);*/
	writeString(ROUTER_ID_EEPROM_ADDR + 2, v, 20/*len*/);


	  //unsigned char saved[MAX_WIFI_LENGTH + 1] = { 0 };
	  // v.getBytes(saved, sizeof(saved));
	  // saved[MAX_WIFI_LENGTH] = '\0';
	  // char* msg = (char*)saved;
	  // mem.write(ROUTER_ID_EEPROM_ADDR, CORRECT_DATA);
	  // mem.writeChars(ROUTER_ID_EEPROM_ADDR + 1, msg, sizeof(msg));
   //    mem.write(ROUTER_ID_EEPROM_ADDR + 2 + sizeof(msg), 0);

}
//--------------------------------------------------------------------------------------------------------------------------------------
String SettingsClass::GetRouterPassword()
{
    byte v_length = byte(mem.read(ROUTER_PASSWORD_EEPROM_ADDR + 1));
    routerPassword = readString(ROUTER_PASSWORD_EEPROM_ADDR + 2, 20/*v_length*/);
    return routerPassword;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::SetRouterPassword(const String& v)
{
    // routerPassword = v;

    byte len = v.length();
    writeString(ROUTER_PASSWORD_EEPROM_ADDR + 2, v, 20/*len*/);

}

//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::setup()
{

	// настраиваем "подхват питания"
    pinMode(POWER_ON_IN, INPUT);
    digitalWrite(POWER_ON_IN, HIGH);         // Подключаем к кнопке включения питания подтягиваючий резистор 
    pinMode(POWER_ON_OUT, OUTPUT);
	pinMode(POWER_BATTERY, INPUT);
	pinMode(BATTERY_CHARGE, INPUT);
    pinMode(LCD_LED, OUTPUT);                // Подсветка дисплея
    digitalWrite(LCD_LED, HIGH);             // 


	pinMode(KEY_REF_OFF, OUTPUT);            // Ключ блокировки
	digitalWrite(KEY_REF_OFF, HIGH);         // 

    pinMode(TOUCH_IRQ, INPUT);               // pin индикации нажатия тачскрина
    digitalWrite(TOUCH_IRQ, HIGH);           // Подключаем к подтягиваючий резистор 

	
    // проверяем тип питания
    checkPower();

    powerButton.begin(POWER_ON_IN, true, 3000U);


#ifdef USE_DS3231_REALTIME_CLOCK

    if (!_rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
    }



#endif
   /* wifiState = read8(WIFI_STATE_EEPROM_ADDR, 0x01);
    routerID = readString(ROUTER_ID_EEPROM_ADDR, 20);
    routerPassword = readString(ROUTER_PASSWORD_EEPROM_ADDR, 20);
    stationID = readString(STATION_ID_EEPROM_ADDR, 20);
    stationPassword = readString(STATION_PASSWORD_EEPROM_ADDR, 20);*/

	/*String routerID2 = Settings.GetRouterID();
	String routerPassword2 = Settings.GetRouterPassword();

    Serial.print("routerID2..");
	Serial.println(routerID2);
    Serial.print("routerPassword2..");
	Serial.println(routerPassword2);*/

	//char msg[20] = "";

	//mem.readChars(ROUTER_ID_EEPROM_ADDR + 2, msg, sizeof(msg));

	//Serial.print("routerID.. ");
	//Serial.println(msg);

	//mem.readChars(ROUTER_PASSWORD_EEPROM_ADDR + 2, msg, sizeof(msg));
	//Serial.print("routerPassword.. ");
	//Serial.println(msg);


}
//--------------------------------------------------------------------------------------------------------------------------------
void SettingsClass::update()
{

    TFTMenu* menuManager;
    powerButton.update();
    // isClicked
    if (powerButton.isRetention())
    {
        DBGLN(F("POWER KEY DETECTED, TURN POWER OFF!!!"));
        Vector<const char*> lines;
        lines.push_back("Устройство");
        lines.push_back("готово");
        lines.push_back("к отключению.");

        //MessageBox->halt("СООБЩЕНИЕ", lines, NULL);
        MessageBox->halt("СООБЩЕНИЕ", lines, true, true);
        DBGLN(F("POWER OFF!!!"));

#ifdef USE_BUZZER
        Buzzer.buzz();
#endif
        Settings.turnPowerOff();
    }

    if (powerButton.isDoubleClicked())// дважды нажать кнопку для вызова калибровки тача
    {
#ifdef USE_BUZZER
        Buzzer.buzz();
        delay(200);
        Buzzer.buzz();
        delay(200);
        Buzzer.buzz();
#endif

        TFTScreen->switchToScreen("TOUCH_CALIBRATION"); // экран калибровки тача
    }

    static uint32_t tmr = millis();
    if (millis() - tmr > 1000)
    {
        int Power5 = Settings.getPowerVoltage5(POWER_BATTERY);
        tmr = millis();
    }

}


void  SettingsClass::setVer(String ver)
{
    ver_prog = ver;
}

String  SettingsClass::getVer()
{
    return ver_prog;
}



//--------------------------------------------------------------------------------------------------------------------------------
