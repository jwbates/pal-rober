#include <Wire.h>

#include "/Users/jwbates/Projects/pal/src/include/common.h"
#include "/Users/jwbates/Projects/pal/src/include/color_set.h"
#include "/Users/jwbates/Projects/pal/src/include/kmeans_color.h"

bool capture_started = false;
int  bytes_received = 0;
int  num_centroids = 6;

ColorSet            colors;
KMeansColor         kmeans = KMeansColor(num_centroids);

void setup() {
     Serial.begin(9600);                // start serial for output
     Wire.begin();        // join i2c bus 
}
  
void loop() {
     if (!capture_started) {
	  Serial.println("Starting capture");
	  take_picture();
	  return;
     }
  
     read_image_bytes();
}

//
// To take a picture, we have to send a command and then
// wait for the bytes to start flowing.
//
void take_picture()
{
     send_capture_start();
     capture_started = true;
     colors.reset();
     delay(100);
}

void send_capture_start()
{
     Wire.beginTransmission(SLAVE_ADDRESS);
     Wire.write(START_CAPTURE_REQUEST);
     Wire.endTransmission();
}

//
// To read image bytes, we request them
//

void read_image_bytes()
{
     if (!capture_started) {
	  Serial.println("Trying to read when capture not started");
	  return;
     }

     if (bytes_received < IMAGE_BYTES)
     {
	  Wire.requestFrom(SLAVE_ADDRESS, REQUEST_SIZE);
	  int bytes_read = 0;

	  while (Wire.available() && bytes_read < REQUEST_SIZE) {
	       byte first_byte = Wire.read();
	       bytes_received++;
	       bytes_read++;
	       
	       if (!Wire.available()) {
		    Serial.println("Ran out of image bytes in middle of pixel");
		    break;
	       }

	       byte second_byte = Wire.read();
	       bytes_received++;
	       bytes_read++;

	       word full_pixel = (((word) second_byte) << 8) | ((word) first_byte);

	       colors.addColor(full_pixel);

	       if (bytes_received % 10000 == 2) {
		    Serial.print(bytes_received);
		    Serial.println(" bytes received");
	       }
	       if (bytes_received > IMAGE_BYTES)
		    break;
	  }
     }

     if (bytes_received >= IMAGE_BYTES) {
	  Serial.println("Image received");
	  //
	  // make cluster based on image received
	  //
	  kmeans.cluster(colors);
	  kmeans.dumpToSerial();

	  // reset the capture
	  bytes_received = 0;
	  capture_started = false;
	  delay(1000);
     }
}
