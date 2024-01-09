/*


TFT ILI9225 SPI
Dots: 176 x 220

 */

#include <stdio.h>                // define I/O functions
#include <Arduino.h>              // define I/O functions
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
//#include "SPIFFS.h"
//#include "FS.h"
#include "Free_Fonts.h" // Include the header file attached to this sketch
#include <EEPROM.h>
#include "NotoSansMonoSCB20.h"
#include "NotoSansBold15.h"
#include "Final_Frontier_28.h"
#include "Latin_Hiragana_24.h"
#include "NotoSansBold36.h"


TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
TFT_eSprite spr = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object

# define SENSOR_PIN 0

#define WIDTH  210
#define HEIGHT 80
#define DELAY 1000

int val = 0;
int val_tmp = 0;

#define backColor     0x0026
#define gaugeColor    0x055D
#define dataColor     0x0311
#define purple        0xEA16
#define Air_infoColor 0xF811

static uint32_t tmr;

bool volt_pa = false;

char unitID_in[10];
char command_in[10];
char data_in[100];

int adrr_VP = 0;
int adrr_K = 4;
int adrr_H = 8;
int adrr_L = 12;



void setup(void) 
{
  Serial.begin(115200);
  
  Serial.println("Initialized");

  String ver_soft = __FILE__;
  int val_srt = ver_soft.lastIndexOf('\\');
  ver_soft.remove(0, val_srt + 1);
  val_srt = ver_soft.lastIndexOf('.');
  ver_soft.remove(val_srt);
  Serial.println(ver_soft);

  EEPROM.begin(512);             // Инициализация EEPROM с размером 512 байт
 
  tft.init();             // initialize TFT
  tft.setRotation(1);
 
  tft.fillRect(0, 0, 220, 176, backColor);
 
  tft.setFreeFont(FF39);       // Select Free Serif 24 point font
  Startdrawtext("DECIMA", TFT_RED);
  tft.setFreeFont(FF0);       // Select Free
  tft.setTextSize(1);
  Verdrawtext(ver_soft, TFT_WHITE);
  delay(2000);
  tft.fillRect(0, 0, 220, 176, TFT_BLACK);
  Serial.println("done");

  volt_pa = true;

 tmr = millis();

}

void loop() 
{
 
    int i = 0;
    char buffer[100];

    //если есть данные - считаем их
    if (Serial.available()) 
    {
        delay(100);

        //сохраним прочитанное в буфер
        while (Serial.available() && i < 99) { buffer[i++] = Serial.read(); } //закрываем массив buffer[i++]=''; } //если вдруг буфер наполнен if(i>0){  

        sscanf(buffer, "%[^','],%s", &command_in, &data_in);

    }

    
    if ((String)command_in == "V" || (String)command_in == "v")
    {
        Serial.print("command: ");
        Serial.println(command_in);
        byte value = 1;                // значение данных (от 0 до 255)
        EEPROM.write(adrr_VP, value);  // Запись данных
        EEPROM.commit();               // Сохранение изменений
        unitID_in[0] = 0;
        command_in[0] = 0;
    }
    else if ((String)command_in == "P"|| (String)command_in == "p")
    {
        Serial.print("command: ");
        Serial.println(command_in);
        byte value = 0;                // значение данных (от 0 до 255)
        EEPROM.write(adrr_VP, value);  // Запись данных
        EEPROM.commit();               // Сохранение изменений
        unitID_in[0] = 0;
        command_in[0] = 0;
    }
    else if ((String)command_in == "K" || (String)command_in == "k")
    {
        Serial.print("command: ");
        Serial.print(command_in);
        Serial.print(" data ");
        Serial.println(data_in);
        String val_str = data_in;
        float val_K = val_str.toFloat(); //– конвертирует и возвращает содержимое строки в тип данных float
        EEPROM_float_write(adrr_K, val_K);
        unitID_in[0] = 0;
        command_in[0] = 0;
        val_tmp = -1;
 
    }

    else if ((String)command_in == "H" || (String)command_in == "h")
    {
        Serial.print("command: ");
        Serial.print(command_in);
        Serial.print(" data ");
        Serial.println(data_in);
        unitID_in[0] = 0;
        command_in[0] = 0;
        val_tmp = -1;
    }
    else if ((String)command_in == "L" || (String)command_in == "l")
    {
        Serial.print("command: ");
        Serial.print(command_in);
        Serial.print(" data ");
        Serial.println(data_in);
        unitID_in[0] = 0;
        command_in[0] = 0;
        val_tmp = -1;
    }


    if (millis() - tmr > 1000)
    {
        tmr = millis();
        val = read();

        volt_pa = EEPROM.read(0);   // Чтение данных


        if (val_tmp != val)
        {
            val_tmp = val;

            if (volt_pa)
            {

                float voltagePa = ((val * 1.0) / 1024);  // Напряжение питания  

                uint32_t computedU = voltagePa * 100;

                int voltagePa_Value = computedU / 100;
                int voltagePa_Fract = computedU % 100;

                String data;
                data += voltagePa_Value;
                data += '.';
                 if (voltagePa_Fract < 10)
                     data += '0';
                data += voltagePa_Fract;
                data += " V  ";
                spr.createSprite(WIDTH, HEIGHT);
                spr.loadFont(NotoSansBold36);          // Загружаем шрифты символов направления света
                spr.fillSprite(TFT_BLUE);
                spr.drawRect(0, 0, WIDTH, HEIGHT, TFT_WHITE);
                spr.setTextDatum(MC_DATUM);
                spr.setTextColor(TFT_WHITE, TFT_BLUE);
                spr.drawString(data, WIDTH / 2, HEIGHT / 2, 4);
                spr.pushSprite(5, 50);
                spr.deleteSprite();

            }
            else
            {

                uint32_t val_K = EEPROM_float_read(adrr_K);
                uint32_t val_raw  = val * val_K;

                String data;
                data += val_raw;
                data += " Pa  ";

                spr.createSprite(WIDTH, HEIGHT);
                spr.loadFont(NotoSansBold36);          // Загружаем шрифты символов направления света
                spr.fillSprite(TFT_BLUE);
                spr.drawRect(0, 0, WIDTH, HEIGHT, TFT_WHITE);
                spr.setTextDatum(MC_DATUM);
                spr.setTextColor(TFT_WHITE, TFT_BLUE);
                spr.drawString(data, WIDTH / 2, HEIGHT / 2, 4);
                spr.pushSprite(5, 50);
                spr.deleteSprite();

            }
        }
    }
 
    // обрабатываем входящие команды
  
}


uint16_t read()
{
	int ADCVal = 0;
	int ADCRef = 0;
	int ADC_count = 10;


	for (int i = 0; i < ADC_count; i++)  // выполним несколько замеров для верности
	{
		ADCVal += analogRead(SENSOR_PIN);
	}

	ADCVal = ADCVal / ADC_count;                         // Получить исходное напряжение на входе микроконтроллера
	//Serial.print("Sensor analog ... ");
	//Serial.println(ADCVal);

	return ADCVal;                                         // Вернуть напряжение с датчика за вычетом опорного
}



void Startdrawtext(String text, uint16_t color)
{
  tft.setCursor(40, 95);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void Verdrawtext(String text, uint16_t color)
{
    tft.setCursor(25, 155);
    tft.setTextColor(color);
    tft.setTextWrap(true);
    tft.print(text);
}

float EEPROM_float_read(int addr) 
{
    byte raw[4];
    for (byte i = 0; i < 4; i++) raw[i] = EEPROM.read(addr + i);
    float& num = (float&)raw;
    return num;
}

// запись
void EEPROM_float_write(int addr, float num) 
{
    byte raw[4];
    (float&)raw = num;
    for (byte i = 0; i < 4; i++) EEPROM.write(addr + i, raw[i]);
    EEPROM.commit();               // Сохранение изменений
}


/*

#include <EEPROM.h>

void setup() {
    EEPROM.begin(512);  // Инициализация EEPROM с размером 512 байт
    int address = 0;   // адрес памяти для записи (от 0 до 511)
    byte value = 123;  // значение данных (от 0 до 255)
    EEPROM.write(address, value);  // Запись данных
    EEPROM.commit();   // Сохранение изменений
}

#include <EEPROM.h>

void setup() {
    EEPROM.begin(512);
    int address = 0;                     //адрес памяти для чтения.
    byte value = EEPROM.read(address);   // Чтение данных
    // Используйте значение данных (value) по вашему усмотрению
}

1. EEPROM.begin(size):

Инициализирует эмулированную EEPROM с указанным размером size (в байтах). Обычно вызывается в функции setup().

2. EEPROM.end():

Завершает работу с эмулированной EEPROM и освобождает зарезервированные ресурсы. Необходимо вызывать, когда вы закончили работу с EEPROM.

3. EEPROM.write(address, value):

Записывает значение value (байт) в указанный адрес address в эмулированной EEPROM.

4. EEPROM.read(address):

Считывает байт данных из указанного адреса address в эмулированной EEPROM и возвращает это значение.

5. EEPROM.update(address, value):

Обновляет значение в указанном адресе address в эмулированной EEPROM. Если новое значение отличается от текущего, то происходит запись нового значения.

6. EEPROM.get(address, data):

Считывает данные из указанного адреса address в эмулированной EEPROM и сохраняет их в переменную data. Может использоваться для считывания структур или пользовательских типов данных.

7. EEPROM.put(address, data):

Записывает данные из переменной data в указанный адрес address в эмулированной EEPROM. Может использоваться для записи структур или пользовательских типов данных.

8. EEPROM.commit():

Сохраняет все изменения, внесенные в эмулированную EEPROM, во флэш-памяти. Это необходимо вызвать после операций записи, чтобы изменения вступили в силу.




*/