#ifndef hexagonClass_h
#define hexagonClass_h

#include "Arduino.h"
#include "stdlib.h"
#include <eHealth.h>


// Library interface description
class hexagonClass {
	private:
		int dataContainer;

	
	public: 
		
		// Sensors
		static const int ECG = 6;
		static const int EMG = 7;
		static const int AIRFLOW = 4;
		static const int GALVANIC = 5;
		static const int TEMP = 1;
		static const int PULSE = 3;
		static const int BLOOD = 2;
		static const int POSITION = 8;
		static const int GLUCO = 0;
		
		//	Commands
		static const int CTRL = 0;
		static const int STATUS = 1;
		
		//	Operations
		static const int GET = 0;
		
		//	Params
		static const int BATTERYLEVEL = 9;
		static const int AVAILABLE = 10;
		static const int CONNECTIVITY = 11;
		
		// Responses
		static const int CONNECTED = 0;
		static const int DISCONNECTING = 1;

		//	Containers
		static const int XMLCONTAINER = 0;
		static const int JSONCONTAINER = 1;

		hexagonClass(void);
		
		/**
		 * [hexagonClass::initWireless Initialise the wireless connection between Arduino and Android]
		 * @param type [Bluetooth or Wifi]
		 * @param name [Friednly name for the Arduino]
		 * @param pin  [pin number for the Bluetooth communication]
		 */
		
		void initBluetooth(char access_point_name[], unsigned long p);
		void initWifi(char access_point_name[]);
		void check();
		/**
		 * [hexagonClass::dataAvailable Available data on the wireless link]
		 * @return [length of available data]
		 */
		
		int dataAvailable();

		String xmlMessageAnalog(String type, String sensor, float value);
		String xmlMessageDigital(String type, String sensor, unsigned long value);
		String jsonMessageAnalog(String type, String sensor, float value);
		String jsonMessageDigital(String type, String sensor, unsigned long value);

		void send(String msg);

		int returnCommand(String data);
		int returnOperation(String data);
		int returnParams(String data);
		int returnContainer(String data);
		long readVcc();
		long batteryLevel();
		unsigned long availableSensors();
		int checkSensor(int i);
		char* splitter(String message, int token);
};
extern hexagonClass hexagon;
#endif