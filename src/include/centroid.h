#ifndef _CENTROID_H
#define _CENTROID_H

#include "color.h"

class Centroid {
public:
     Centroid()
	  {
	       _color.randomize();
	       _member_count = 0;
	  }
  
     Centroid(const Color & color) :
	  _color(color), _member_count(0)
	  {
	  }

     const Color & color() const
	  {
	       return _color;
	  }

     double randomize()
	  {
	       _color.randomize();
	       _member_count = 0;
	  }

     double distance(const Color & color) const
	  {
	       return _color.distance(color);
	  }

     void add_to_cluster(const Color & color, const int count = 1)
	  {
	       double red = (double) color.red();
	       double green = (double) color.green();
	       double blue = (double) color.blue();

	       _red_sum += count * red;
	       _green_sum += count * green;
	       _blue_sum += count * blue;

	       _member_count += count;

	       return;
	  }

     double update()
	  {
	       if (_member_count == 0)
		    return 0.0;
	
	       double red_average = _red_sum/_member_count;
	       double green_average = _green_sum/_member_count;
	       double blue_average = _blue_sum/_member_count;
	       Color lastColor(_color);
    
	       _color.red((uint8_t) red_average);
	       _color.green((uint8_t) green_average);
	       _color.blue((uint8_t) blue_average);

	       _red_sum = _green_sum = _blue_sum = 0.0;
	       _member_count = 0;

	       return lastColor.distance(_color);
	  }

private:
     Color _color;
     int _member_count = 0;

     double _red_sum = 0.0;
     double _green_sum = 0.0;
     double _blue_sum = 0.0;
};

#endif  /* _CENTROID_H */
