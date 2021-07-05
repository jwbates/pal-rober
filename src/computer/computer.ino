#include <Wire.h>

#include "/Users/jwbates/Projects/pal/src/include/common.h"
#include "/Users/jwbates/Projects/pal/src/include/color_set.h"
#include "/Users/jwbates/Projects/pal/src/include/kmeans_color.h"
#include "/Users/jwbates/Projects/pal/src/include/bitmap.h"

bool capture_started = false;
int  bytes_received = 0;
int  num_centroids = 6;

ColorSet            colors;
KMeansColor         kmeans = KMeansColor(num_centroids);
Bitmap              bitmap(IMAGE_HEIGHT, IMAGE_WIDTH);

void setup() {
     Serial.begin(9600);                // start serial for output
     Wire.begin(SLAVE_ADDRESS);        // join i2c bus

     Wire.onRequest(requestEvent);
     Wire.onReceive(receiveEvent);
}
  
void requestEvent()
{
     for (int i = 0; i < CLUSTER_COUNT; i++)
     {
	  uint16_t val = (uint16_t) kmeans.color(i);
	  Serial.println(val);
	  Wire.write(*((byte *) &val));
	  Wire.write(*((byte *) &val + 1));
     }
     kmeans.reset();
}

void receiveEvent(int byte_count)
{
     for (int i = 0; i < byte_count && Wire.available(); i++)
     {
	  byte first = Wire.read();
	  if (!Wire.available()) {
	       Serial.println("Ran out of image bytes in middle of pixel");
	       break;
	  }
	  byte second = Wire.read();
	  word full_pixel = (((word) second) << 8) | ((word) first);

	  bitmap.append(full_pixel);
     }

     if (bitmap.ready())
     {
	  Serial.println("Full bitmap received");
	  kmeans.cluster(bitmap);
     }
}

void loop() {
}
