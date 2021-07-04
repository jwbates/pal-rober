
#include <memorysaver.h>
#include <ArduCAM.h>
#include <Wire.h>
#include <SPI.h>

#include "/Users/jwbates/Projects/pal/src/include/common.h"

const int CS = 7;
ArduCAM camera(OV2640, CS);
bool capture_requested = false;

void setup() {
     set_comms();
     init_camera_spi();
     init_camera();

     // Initialize the servant code
     
     Wire.onRequest(requestEvent);
     Wire.onReceive(receiveEvent);
}

void
receiveEvent(int numBytes)
{
     camera.flush_fifo();
     camera.clear_fifo_flag();
     //Start capture
    camera.start_capture();
    Serial.println("Starting capture");
    capture_requested = true;
}

void
requestEvent()
{
     static int bytes_to_go = 0;
     
     if (capture_requested && camera.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK))
     {
	  if (bytes_to_go == 0)
	  {
	       bytes_to_go = camera.read_fifo_length();
	  }
	  int bytes_to_read = REQUEST_SIZE;
	  if (bytes_to_read > bytes_to_go)
	       bytes_to_read = bytes_to_go;
	  
	  Serial.print("Reading ");
	  Serial.print(REQUEST_SIZE, DEC);
	  Serial.println(" bytes");

	  camera.CS_LOW();
	  camera.set_fifo_burst();

	  byte bytes[REQUEST_SIZE];

	  SPI.transfer(bytes, REQUEST_SIZE);
	  for (int i = 0; i < bytes_to_read; i++)
	       bytes[i] = SPI.transfer(0x00);

	  Wire.write(bytes, REQUEST_SIZE);

	  camera.CS_HIGH();
	  bytes_to_go -= bytes_to_read;
	  if (bytes_to_go <= 0) {
	       camera.clear_fifo_flag();
	       capture_requested = false;
	  }
     }
}

void loop() {
}

void set_comms()
{
     Wire.begin(SLAVE_ADDRESS);
     Serial.begin(9600);

     Serial.println(F("ACK CMD ArduCAM Start! END"));

     // set the CS as an output:
     pinMode(CS, OUTPUT);
     digitalWrite(CS, HIGH);
}

void init_camera_spi()
{
     uint8_t temp;

     SPI.begin();
     //Reset the CPLD
     camera.write_reg(0x07, 0x80);
     delay(100);
     camera.write_reg(0x07, 0x00);
     delay(100);
  
     while(1) {
	  //Check if the ArduCAM SPI bus is OK
	  camera.write_reg(ARDUCHIP_TEST1, 0x55);
	  temp = camera.read_reg(ARDUCHIP_TEST1);
	  if (temp != 0x55){
	       Serial.println(F("ACK CMD SPI interface Error! END"));
	       delay(1000);
	       continue;
	  } else {
	       Serial.println(F("ACK CMD SPI interface OK. END"));break;
	  }
     }
}

void init_camera()
{
     uint8_t vid, pid;

     while(1){
	  //Check if the camera module type is OV2640
	  camera.wrSensorReg8_8(0xff, 0x01);
	  camera.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
	  camera.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
	  if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
	       Serial.println(F("ACK CMD Can't find OV2640 module! END"));
	       delay(1000);
	       continue;
	  }
	  else{
	       Serial.println(F("ACK CMD OV2640 detected. END"));
	       break;
	  } 
     }
     delay(1000);
     camera.clear_fifo_flag();
     camera.set_format(BMP);
     camera.InitCAM();
}
