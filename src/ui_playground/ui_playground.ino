#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#include "/Users/jwbates/Projects/pal/src/include/screen.h"

Screen * screen;

void setup(void) {
  Serial.begin(9600);
  Serial.println("Started");
  screen = new Screen();
  pinMode(13, OUTPUT);
}

void loop(void)
{
     screen->doLoop();
}
