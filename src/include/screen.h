#ifndef _SCREEN_H
#define _SCREEN_H

#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#include "/Users/jwbates/Projects/pal/src/include/common.h"
#include "/Users/jwbates/Projects/pal/src/include/color.h"

#define MIN_PRESSURE 10

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

#define	BLACK   0x0000
#define	RED     0xF800
#define GREY    0x8210

class Screen
{
public:
     Screen() :
	  _ts(XP, YP, XM, YM, 300),
	  _tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET)
	  {
	       _tft.reset();
	       _tft.begin(_tft.readID());
	       _tft.fillScreen(BLACK);

	       _height = _tft.height();
	       _width = _tft.width();

	       Serial.print("Width: "); Serial.print(_width);
	       Serial.print(" Height: "); Serial.print(_height);

	       _buttonActive = true;
	       _wasTouching = false;
	       _needsRedraw = true;

	       // Button is twice the height of the swatches.
	       
	       _swatchHeight = _height/(CLUSTER_COUNT + 2);
	       _buttonHeight = 2*_swatchHeight;

	       for (short i = 0; i < CLUSTER_COUNT; i++)
	       {
		    _colors[i].randomize();
	       }
	  }

     void doLoop()
	  {
	       _checkButtonStatus();
	       if (_needsRedraw)
	       {
		    _drawButton();
		    for (short i = 0; i < CLUSTER_COUNT; i++)
			 _drawSwatch(i);
	       }
	       _needsRedraw = false;
	  }

private:
     TouchScreen      _ts;
     Elegoo_TFTLCD    _tft;
     int16_t          _height;
     int16_t          _width;

     TSPoint          _lastPointTouched;
     bool             _wasTouching;
     bool             _buttonActive;

     Color            _colors[CLUSTER_COUNT];

     int16_t          _buttonHeight;
     int16_t          _swatchHeight;

     bool             _needsRedraw;

     void _checkButtonStatus()
	  {
	       static int last_pressure = 0;
	       
	       if (_buttonActive)
	       {
		    digitalWrite(13, HIGH);
		    TSPoint currentPoint = _ts.getPoint();
		    digitalWrite(13, LOW);

		    pinMode(XM, OUTPUT);
		    pinMode(YP, OUTPUT);
		    
		    if (last_pressure != 0 && currentPoint.z == 0) {
			 last_pressure = 0;
			 return;
		    }
		    last_pressure = currentPoint.z;
		    
		    if (currentPoint.z > MIN_PRESSURE)
		    {
			 _lastPointTouched = currentPoint;
			 Serial.println("Started press");
			 _wasTouching = true;
			 Serial.print(currentPoint.z);
			 Serial.println(" pressure");
		    } else {
			 if (_wasTouching)
			 {
			      Serial.println("Ended press");
			      Serial.print(currentPoint.z);
			      Serial.println(" pressure");
			      _wasTouching = false;
			      _handlePossibleButtonPress();
			 }
		    }
	       }
	  }

     void _drawButton()
	  {
	       _tft.fillRect(0, 0, _width, _buttonHeight, BLACK);
	       int color = _buttonActive?RED:GREY;
	       
	       _tft.fillCircle(_width/2, _buttonHeight/2, _buttonHeight/2-5, color);
	  }

     void _drawSwatch(short index)
	  {
	       int color = (int) _colors[index];
	       int base_y = _buttonHeight + index * _swatchHeight;

	       _tft.fillRect(0, base_y+5, _width, base_y + _swatchHeight-5, color);
	  }

     void _handlePossibleButtonPress()
	  {
	       int y = map(_lastPointTouched.y, 120, 940, _height, 0);
	       Serial.print(y);
	       Serial.println(" last y");
	       if (y > _swatchHeight)
	       {
		    Serial.println("BUTTON PRESSED");
		    _needsRedraw = true;
	       }
	  }
};

#endif /* _SCREEN_H */
