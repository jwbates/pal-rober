#ifndef _COLOR_SET_H
#define _COLOR_SET_H

#define MAX_COLORS      512
#define COLOR_CACHE     16

#include "color.h"

class ColorSetEntry {
public:
     ColorSetEntry() : _color(0), _count(0) {}
  
     const bool operator==(const word color) const
	  {
	       return _color == color;
	  }
  
     void color(const word color)
	  {
	       _color = color;
	       _count++;
	  }

     void increment()
	  {
	       _count++;
	  }

     word color() const
	  {
	       return _color;
	  }
  
     int count() const
	  {
	       return _count;
	  }

     void reset()
	  {
	       _color = 0;
	       _count = 0;
	  }
    
private:
     word     _color;
     int      _count;
};

typedef struct ColorCacheEntry
{
     word     color;
     int      index;
} ColorCacheEntry;

class ColorSet
{
public:
     ColorSet()
	  {
	       reset();
	  }

     short maxIndex() const
	  {
	       return MAX_COLORS;
	  }
  
     void addColor(const word color)
	  {
	       int i;
	       bool cache_slots_open = false;
	       
	       for (i = 0; i < COLOR_CACHE; i++)
	       {
		    if (_cache[i].index == -1)
			 cache_slots_open = true;
		    
		    if ((_cache[i].color == color) && (_cache[i].index != -1))
		    {
			 _colors[_cache[i].index].increment();
			 _last_cached = i;
			 return;
		    } 
	       }
	       
	       for (i = 0; i < MAX_COLORS; i++)
	       {
		    if (_colors[i] == color)
		    {
			 _colors[i].increment();
			 break;
		    }
		    else if (_colors[i].count() == 0) {
			 _colors[i].color(color);
		    }
	       }

	       if (i < MAX_COLORS)
	       {
		    int next_entry = (_last_cached + 1) % COLOR_CACHE;
		    if (cache_slots_open) {
			 for (int i = 0; i < COLOR_CACHE; i++)
			 {
			      if (_cache[i].index == -1)
			      {
				   next_entry = i;
				   break;
			      }
			 }
		    }
		    _cache[next_entry].color = color;
		    _cache[next_entry].index = i;
		    _last_cached = next_entry;
	       }
	  }

     Color colorAt(const short index) const
	  {
	       return Color(_colors[index].color());
	  }
     
     int countAt(const short index) const
	  {
	       return _colors[index].count();
	  }
     
     void reset()
	  {
	       for (int i = 0; i < MAX_COLORS; i++)
	       {
		    _colors[i].reset();
	       }

	       for (int i = 0; i < COLOR_CACHE; i++)
	       {
		    _cache[i].index = -1;
	       }
	       _last_cached = -1;
	  }
     
private:
     ColorSetEntry         _colors[MAX_COLORS];
     ColorCacheEntry       _cache[COLOR_CACHE];
     int                   _last_cached; 
};

#endif /* _COLOR_SET_H */
