
#include <Wire.h>
#include <SPI.h>

#include "/Users/jwbates/Projects/pal/src/include/common.h"
#include "/Users/jwbates/Projects/pal/src/include/camera.h"
#include "/Users/jwbates/Projects/pal/src/include/screen.h"

Camera * camera;
Screen * screen;

void setup() {
     set_comms();
     camera = new Camera();
     screen = new Screen(*camera);
}

void loop() {
     screen->doLoop();
     if (!camera->done())
     {
	  camera->update(SLAVE_ADDRESS);
	  if (camera->done()) {
	       delay(500);
	       get_colors();
	  }
     }
}

void set_comms()
{
     Wire.begin();
     Serial.begin(9600);

     Serial.println(F("ACK CMD ArduCAM Start! END"));
}

void get_colors()
{
     int bytes_to_read = CLUSTER_COUNT * BYTES_PER_PIXEL;
     int bytes_read = 0;
     short colors_read = 0;
     
     Wire.requestFrom(SLAVE_ADDRESS, CLUSTER_COUNT * BYTES_PER_PIXEL);

     while (Wire.available() && bytes_read < bytes_to_read)
     {
	  byte first_byte = Wire.read();
	  bytes_read++;

	  if (!Wire.available()) {
	       Serial.println("Ran out of image bytes in middle of pixel");
	       break;
	  }

	  byte second_byte = Wire.read();
	  bytes_read++;
	  
	  word full_pixel = (((word) second_byte) << 8) | ((word) first_byte);
	  Serial.println(full_pixel);
	  Color color(full_pixel);
	  screen->setColor(colors_read++, color);
     }
}
