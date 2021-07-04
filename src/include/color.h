#ifndef _COLOR_H
#define _COLOR_H

#include <math.h>

#define RED_MASK    0xF800
#define GREEN_MASK  0x07E0
#define BLUE_MASK   0x001F

#define RED_PIXEL(val)    ((val & RED_MASK) >> 11) << 3
#define GREEN_PIXEL(val)  ((val & GREEN_MASK) >> 5) << 3
#define BLUE_PIXEL(val)   ((val & BLUE_MASK)) << 3

class Color
{
public:
     Color() {}

     Color(uint8_t red, uint8_t green, uint8_t blue) :
	  _red(red), _green(green), _blue(blue)
	  {
	  }

     Color(word pixel):
	  _red(RED_PIXEL(pixel)),
	  _green(GREEN_PIXEL(pixel)),
	  _blue(BLUE_PIXEL(pixel))
	  {
	  }
     Color(const Color & other):
	  _red(other._red), _green(other._green), _blue(other._blue)
	  {}

     void dumpToSerial() const
	  {
	       Serial.print("Red: ");
	       Serial.print(red());
	       Serial.print(" Blue: ");
	       Serial.print(blue());
	       Serial.print(" Green: ");
	       Serial.println(green());
	  }
  
     void randomize()
	  {
	       _red = (uint8_t) random(256);
	       _green = (uint8_t) random(256);
	       _blue = (uint8_t) random(256);
	  }

     const bool operator==(const Color & other) const
	  {
	       return (red() == other.red() &&
		       green() == other.green() &&
		       blue() == other.blue());
	  }

     const bool operator!=(const Color & other) const
	  {
	       return !(*this == other);
	  }

     const Color & operator=(const Color & other)
	  {
	       _red = other.red();
	       _green = other.green();
	       _blue = other.blue();
	       
	       return *this;
	  }

     const double distance(const Color & other) const
	  {
	       double red = (double) (this->red() - other.red());
	       double green = (double) (this->green() - other.green());
	       double blue = (double) (this->blue() - other.blue());

	       double squared = red * red + green * green + blue*blue;

	       return sqrt(squared);
	  }

     const uint8_t red() const
	  {
	       return _red;
	  }

     const void red(const uint8_t val)
	  {
	       _red = val;
	  }

     const uint8_t green() const
	  {
	       return _green;
	  }

     const void green(const uint8_t val)
	  {
	       _green = val;
	  }
     
     const uint8_t blue() const
	  {
	       return _blue;
	  }
  
     const void blue(const uint8_t val)
	  {
	       _blue = val;
	  }

private:
     uint8_t _red = 0;
     uint8_t _green = 0;
     uint8_t _blue = 0;
};

#endif /* _COLOR_H */
