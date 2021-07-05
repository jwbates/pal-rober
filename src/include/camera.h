#ifndef _CAMERA_H
#define _CAMERA_H

#include <memorysaver.h>
#include <ArduCAM.h>
#include <Wire.h>
#include <SPI.h>

#define CS 10

class Camera
{
public:
     Camera():
	  _camera(OV2640, CS),
	  _captureActive(false),
	  _bytes_to_go(-1)
     {
	  _init_camera_spi();
	  _init_camera();
	  // set the CS as an output:
	  pinMode(CS, OUTPUT);
	  digitalWrite(CS, HIGH);
     }

     void start()
	  {
	       if (_captureActive)
	       {
		    Serial.println("Capture already active");
		    return;
	       }

	       _camera.flush_fifo();
	       _camera.clear_fifo_flag();
	       _camera.start_capture();
	       _captureActive = true;
	       Serial.println("Starting capture");
	  }

     void update(int toAddress)
	  {
	       if (!_camera.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK))
		    return;
	       
	       if (_captureActive && _bytes_to_go < 0)
	       {
		    _bytes_to_go = _camera.read_fifo_length();
	       }

	       _camera.CS_LOW();
	       _camera.set_fifo_burst();

	       byte bytes[REQUEST_SIZE];

	       for (int i = 0; i < REQUEST_SIZE; i++)
		    bytes[i] = SPI.transfer(0x00);

	       Wire.beginTransmission(toAddress);
	       Wire.write(bytes, REQUEST_SIZE);
	       Wire.endTransmission();

	       _camera.CS_HIGH();

	       _bytes_to_go -= REQUEST_SIZE;
	       if (_bytes_to_go <= 0)
	       {
		    _bytes_to_go = -1;
		    _captureActive = false;
	       }
	  }

     bool done() const
	  {
	       return (!_captureActive && _bytes_to_go < 0);
	  }

private:
     ArduCAM  _camera;
     bool     _captureActive;
     int      _bytes_to_go;

     void _init_camera_spi()
	  {
	       uint8_t temp;

	       SPI.begin();
	       //Reset the CPLD
	       _camera.write_reg(0x07, 0x80);
	       delay(100);
	       _camera.write_reg(0x07, 0x00);
	       delay(100);
  
	       while(1) {
		    //Check if the ArduCAM SPI bus is OK
		    _camera.write_reg(ARDUCHIP_TEST1, 0x55);
		    temp = _camera.read_reg(ARDUCHIP_TEST1);
		    if (temp != 0x55){
			 Serial.println(F("ACK CMD SPI interface Error! END"));
			 delay(1000);
			 continue;
		    } else {
			 Serial.println(F("ACK CMD SPI interface OK. END"));break;
		    }
	       }
	  }

     void _init_camera()
	  {
	       uint8_t vid, pid;

	       while(1){
		    //Check if the camera module type is OV2640
		    _camera.wrSensorReg8_8(0xff, 0x01);
		    _camera.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
		    _camera.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
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
	       _camera.clear_fifo_flag();
	       _camera.set_format(BMP);
	       _camera.InitCAM();
	  }
};

#endif /* _CAMERA_H */
