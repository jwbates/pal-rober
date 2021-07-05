#ifndef _KMEANS_COLOR_H
#define _KMEANS_COLOR_H

#include "/Users/jwbates/Projects/pal/src/include/centroid.h"
#include "/Users/jwbates/Projects/pal/src/include/bitmap.h"
#include "/Users/jwbates/Projects/pal/src/include/color_counter.h"

//
// UNDONE is just a large value to be used as the 'maximum minimum' while searching for the
// actual minimum.
//

#define UNDONE         20000.0

//
// MAX_CLUSTERS is the maximum number of centroids to allocate.
//
#define MAX_CLUSTERS   8

#define DEFAULT_CONVERGENCE 0.0001

class KMeansColor
{
public:
     KMeansColor(byte numCentroids, short max_iterations=300, double convergence=DEFAULT_CONVERGENCE) :
	  _numCentroids(numCentroids), _maxIterations(max_iterations), _convergence(convergence)
	  {
	       randomSeed(analogRead(7));
	       
	       for (byte i = 0; i < numCentroids; i++)
	       {
		    _centroids[i].randomize();
	       }
	  }

     void dumpToSerial() const
	  {
	       Serial.println("Cluster centroids:");
	       for (short i = 0; i < _numCentroids; i++)
		    _centroids[i].color().dumpToSerial();
	  }
    
     void cluster(const ColorCounter & counter)
	  {
	       short iterations_so_far;

	       _pickCentroids(counter);

	       Serial.println("Starting clustering");
	       for (iterations_so_far = 0;
		    iterations_so_far < _maxIterations;
		    iterations_so_far++)
	       {
		    for (int i = 0; i < counter.maxIndex(); i++)
		    {
			 if (counter.count(i) == 0)
			      continue;
			 
			 this->_addColor(i, counter.count(i));
			 if (i % 10000 == 0) {
			      Serial.print("sub-iteration: ");
			      Serial.println(i);
			 }
		    }
	    
		    if (this->update()) {
			 Serial.println("Cluster converged");
			 break;
		    }

		    Serial.print("System finished iteration: ");
		    Serial.println(iterations_so_far);
	       }

	       if (iterations_so_far >= _maxIterations)
		    Serial.println("Did not converge");
	  }

     void reset()
	  {
	       for (byte i = 0; i < _numCentroids; i++)
	       {
		    _centroids[i].randomize();
	       }
	  }

     const Color closestMatch(const Color & color, const ColorCounter & counter)
	  {
	       double min_distance = UNDONE;
	       Color min_color;

	       for (int i = 0; i < counter.maxIndex(); i++)
	       {
		    if (counter.count(i) == 0)
			 continue;

		    double distance = color.distance(Color(i));
		    if (distance < min_distance) {
			 min_distance = distance;
			 min_color = Color(i);
		    }
	       }

	       return min_color;
	  }

     // returns true if converged
     bool update()
	  {
	       bool converged = true;

	       for (byte i = 0; i < _numCentroids; i++) {
		    if (_centroids[i].update() > _convergence) 
			 converged = false;
	       }

	       return converged;
	  }

     const Color & color(byte centroidNum) const
	  {
	       return _centroids[centroidNum].color();
	  }

private:
     Centroid   _centroids[MAX_CLUSTERS];
     byte       _numCentroids;
     short      _maxIterations;
     double     _convergence;
     
     void _addColor(const Color & color, const int count = 1)
	  {
	       Centroid * closest;
	       double min = UNDONE;

	       for (byte i = 0; i < _numCentroids; i++)
	       {
		    double distance = _centroids[i].distance(color);
		    if (distance < min)
		    {
			 closest = &(_centroids[i]);
			 min = distance;
		    }
	       }
	       
	       closest->add_to_cluster(color, count);
	  }

     void _pickCentroids(const ColorCounter & counter)
	  {
	       int active_count = 0;

	       for (int i = 0; i < counter.maxIndex(); i++)
		    if (counter.count(i) > 0)
			 active_count++;

	       for (byte i = 0; i < _numCentroids; i++)
	       {
		    int selection = random(active_count);
		    int so_far = 0;
		    for (int j = 0; j < counter.maxIndex(); j++)
		    {
			 if (counter.count(j) == 0)
			      continue;
			 if (selection == so_far)
			 {
			      _centroids[i].color() = (word) j;
			      break;
			 } else {
			      so_far++;
			 }
		    }
	       }
	  }

};

#endif
