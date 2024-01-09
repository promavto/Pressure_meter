#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------
#include "Configuration_ESP32.h"
#include "TFT_eSPI.h"
#include <SPI.h>
//--------------------------------------------------------------------------------------------------------------------------------------
#include "Fonts/GFXFF/gfxfont.h"
#include "BigRusFont.h"
#include "SmallRusFont.h"
#include "IconsFont.h"
#include "SensorFont.h"
#include "SensorFont2.h"
#include "SevenSegNumFontMDS.h"
#include "SevenSegNumFontPlus.h"
#include "Various_Symbols_32x32.h"
#include "Free_Fonts.h" // Include the header file attached to this sketch

#define FONTTYPE const GFXfont*
//--------------------------------------------------------------------------------------------------------------------------------------
#define TFT_Class TFT_eSPI               // класс поддержки TFT
#define TOUCH_Class TFT_eSPI             // класс поддержки тача

#define TFT_FONT (&BigRusFont)           // какой шрифт юзаем
#define TFT_SMALL_FONT (&SmallRusFont)   // какой шрифт юзаем
#define SENSOR_FONT (&SensorFont)
#define SENSOR_FONT2 (&SensorFont2)
#define SEVEN_SEG_NUM_FONT_MDS (&SevenSegNumFontMDS)
#define SEVEN_SEG_NUM_FONT_PLUS (&SevenSegNumFontPlus)
#define VARIOUS_SYMBOLS_32x32 (&Various_Symbols_32x32)
#define ICONS_FONT (&IconsFont)

