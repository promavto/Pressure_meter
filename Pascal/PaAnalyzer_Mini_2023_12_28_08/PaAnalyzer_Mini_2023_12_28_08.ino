/*


TFT ILI9225 SPI
Dots: 176 x 220

 */

#include <stdio.h>                // define I/O functions
#include <Arduino.h>              // define I/O functions
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

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

  EEPROM.begin(128);             // ������������� EEPROM � �������� 512 ����
 
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

 tmr = millis();

}

void loop() 
{
 
    int i = 0;
    char buffer[100];

    //���� ���� ������ - ������� ��
    if (Serial.available()) 
    {
        delay(100);

        //�������� ����������� � �����
        while (Serial.available() && i < 99) { buffer[i++] = Serial.read(); } //��������� ������ buffer[i++]=''; } //���� ����� ����� �������� if(i>0){  

        sscanf(buffer, "%[^','],%s", &command_in, &data_in);

    }

    
    if ((String)command_in == "V" || (String)command_in == "v")
    {
        Serial.print("command: ");
        Serial.println(command_in);
        byte value = 1;                // �������� ������ (�� 0 �� 255)
        EEPROM.write(adrr_VP, value);  // ������ ������
        EEPROM.commit();               // ���������� ���������
        unitID_in[0] = 0;
        command_in[0] = 0;
    }
    else if ((String)command_in == "P"|| (String)command_in == "p")
    {
        Serial.print("command: ");
        Serial.println(command_in);
        byte value = 0;                // �������� ������ (�� 0 �� 255)
        EEPROM.write(adrr_VP, value);  // ������ ������
        EEPROM.commit();               // ���������� ���������
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
        float val_K = val_str.toFloat(); //� ������������ � ���������� ���������� ������ � ��� ������ float
        EEPROM_float_write(adrr_K, val_K);
        unitID_in[0] = 0;
        command_in[0] = 0;
        val_tmp = -1;
 
    }

    else if ((String)command_in == "High" || (String)command_in == "high")
    {
        Serial.print("command: ");
        Serial.print(command_in);
        Serial.print(" data ");
        Serial.println(data_in);
        unitID_in[0] = 0;
        command_in[0] = 0;
        val_tmp = -1;
    }
    else if ((String)command_in == "Low" || (String)command_in == "low")
    {
        Serial.print("command: ");
        Serial.print(command_in);
        Serial.print(" data ");
        Serial.println(data_in);
        unitID_in[0] = 0;
        command_in[0] = 0;
        val_tmp = -1;
    }
    else if ((String)command_in == "Help" || (String)command_in == "help")
    {
        Serial.print("command: ");
        Serial.println(command_in);
        Serial.println();

        Serial.println("Help, or help,     - list of commands");
        Serial.println("V, or v,           - display voltage");
        Serial.println("P, or p,           - display in pascals");
        Serial.println("K or k, data       - entering a coefficient");
        Serial.println("High or high, data - entering an upper threshold (not implemented)");
        Serial.println("Low or low, data   - entering the lower threshold (not implemented)");
        unitID_in[0] = 0;
        command_in[0] = 0;
        val_tmp = -1;
    }


    if (millis() - tmr > 1000)
    {
        tmr = millis();
        val = read();

        volt_pa = EEPROM.read(0);   // ������ ������


        if (val_tmp != val)
        {
            val_tmp = val;

            if (volt_pa)
            {

                float voltagePa = ((val * 1.0) / 1024);  // ���������� �������  

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
                spr.loadFont(NotoSansBold36);          // ��������� ������ �������� ����������� �����
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
                spr.loadFont(NotoSansBold36);          // ��������� ������ �������� ����������� �����
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
 
    // ������������ �������� �������
  
}


uint16_t read()
{
	int ADCVal = 0;
	int ADCRef = 0;
	int ADC_count = 10;


	for (int i = 0; i < ADC_count; i++)  // �������� ��������� ������� ��� ��������
	{
		ADCVal += analogRead(SENSOR_PIN);
	}

	ADCVal = ADCVal / ADC_count;                         // �������� �������� ���������� �� ����� ����������������
	//Serial.print("Sensor analog ... ");
	//Serial.println(ADCVal);

	return ADCVal;                                         // ������� ���������� � ������� �� ������� ��������
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

// ������
void EEPROM_float_write(int addr, float num) 
{
    byte raw[4];
    (float&)raw = num;
    for (byte i = 0; i < 4; i++) EEPROM.write(addr + i, raw[i]);
    EEPROM.commit();               // ���������� ���������
}


/*

#include <EEPROM.h>

void setup() {
    EEPROM.begin(512);  // ������������� EEPROM � �������� 512 ����
    int address = 0;   // ����� ������ ��� ������ (�� 0 �� 511)
    byte value = 123;  // �������� ������ (�� 0 �� 255)
    EEPROM.write(address, value);  // ������ ������
    EEPROM.commit();   // ���������� ���������
}

#include <EEPROM.h>

void setup() {
    EEPROM.begin(512);
    int address = 0;                     //����� ������ ��� ������.
    byte value = EEPROM.read(address);   // ������ ������
    // ����������� �������� ������ (value) �� ������ ����������
}

1. EEPROM.begin(size):

�������������� ������������� EEPROM � ��������� �������� size (� ������). ������ ���������� � ������� setup().

2. EEPROM.end():

��������� ������ � ������������� EEPROM � ����������� ����������������� �������. ���������� ��������, ����� �� ��������� ������ � EEPROM.

3. EEPROM.write(address, value):

���������� �������� value (����) � ��������� ����� address � ������������� EEPROM.

4. EEPROM.read(address):

��������� ���� ������ �� ���������� ������ address � ������������� EEPROM � ���������� ��� ��������.

5. EEPROM.update(address, value):

��������� �������� � ��������� ������ address � ������������� EEPROM. ���� ����� �������� ���������� �� ��������, �� ���������� ������ ������ ��������.

6. EEPROM.get(address, data):

��������� ������ �� ���������� ������ address � ������������� EEPROM � ��������� �� � ���������� data. ����� �������������� ��� ���������� �������� ��� ���������������� ����� ������.

7. EEPROM.put(address, data):

���������� ������ �� ���������� data � ��������� ����� address � ������������� EEPROM. ����� �������������� ��� ������ �������� ��� ���������������� ����� ������.

8. EEPROM.commit():

��������� ��� ���������, ��������� � ������������� EEPROM, �� ����-������. ��� ���������� ������� ����� �������� ������, ����� ��������� �������� � ����.




*/