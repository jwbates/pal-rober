#ifndef _BITMAP_H
#define _BITMAP_H

#include "/Users/jwbates/Projects/pal/src/include/color.h"

class Bitmap
{
public:
     Bitmap(uint16_t height, uint16_t width) :
	  _height(height), _width(width), _current_offset(0), _ready(false)
     {
	  _data_size = _height * _width;
	  _data = new word[_data_size];
     }

     int items() const
     {
	  return _data_size;
     }

     void append(word value)
     {
	  if (_current_offset % 1000 == 0)
	       Serial.println(value);
	  _data[_current_offset] = value;
	  _current_offset++;
	  if (_current_offset == _data_size) {
	       _current_offset = 0;
	       _ready = true;
	  } else {
	       _ready = false;
	  }
     }

     Color get(int index) const
	  {
	       return Color(_data[index]);
	  }

     bool ready() const
	  {
	       return _ready;
	  }

private:
     uint16_t _height;
     uint16_t _width;
     int _data_size;
     int _current_offset;
     bool _ready;
     
     word * _data;
};

#endif /* _BITMAP_H */
