#include <Wire.h>

#include "/Users/jwbates/Projects/pal/src/include/common.h"
#include "/Users/jwbates/Projects/pal/src/include/color_counter.h"
#include "/Users/jwbates/Projects/pal/src/include/kmeans_color.h"
#include "/Users/jwbates/Projects/pal/src/include/bitmap.h"

bool capture_started = false;
int  bytes_received = 0;
int  num_centroids = 6;

KMeansColor         kmeans = KMeansColor(num_centroids);
ColorCounter        counter = ColorCounter();

void setup() {
     Serial.begin(9600);                // start serial for output
     Wire.begin(SLAVE_ADDRESS);        // join i2c bus

     Wire.onRequest(requestEvent);
     Wire.onReceive(receiveEvent);
}
  
void requestEvent()
{
     kmeans.cluster(counter);
     for (int i = 0; i < CLUSTER_COUNT; i++)
     {
	  Color color(kmeans.closestMatch(kmeans.color(i), counter));
	  uint16_t val = (uint16_t)color;
	  Serial.print(val);
	  Serial.print(": ");
	  Serial.print(counter.count(val));
	  Serial.print(" (");Serial.print(color.uint()); Serial.println(")");
	  Wire.write(*((byte *) &val));
	  Wire.write(*((byte *) &val + 1));
     }
     kmeans.reset();
     counter.reset();
}

void receiveEvent(int byte_count)
{
     static int received_count = 0;
     
     for (int i = 0; i < byte_count && Wire.available(); i++)
     {
	  byte first = Wire.read();
	  if (!Wire.available()) {
	       Serial.println("Ran out of image bytes in middle of pixel");
	       break;
	  }
	  byte second = Wire.read();
	  word full_pixel = (((word) second) << 8) | ((word) first);

	  received_count += 2;
	  counter.add(full_pixel);
     }

     if (received_count >= IMAGE_BYTES)
     {
	  Serial.println("Full bitmap received");
	  received_count = 0;
     }
}

void loop() {
}
