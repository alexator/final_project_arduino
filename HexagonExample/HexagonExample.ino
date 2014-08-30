#include <PinChangeInt.h>
#include <eHealth.h>


#include <Hexagon.h>

String data;
char character;
int connected;
uint8_t cont = 0;

void setup() {
        Serial.flush();
	hexagon.initBluetooth("Alex", 12345);
        eHealth.initPulsioximeter();
        
        PCintPort::attachInterrupt(6, readPulsioximeter, RISING);
        delay(1000);
        connected = hexagon.CONNECTED;
        Serial.flush();
}

void loop() {

	while(hexagon.dataAvailable() > 0) {
		character = Serial.read();
		data.concat(character);	
		delay(1);
	}

	if(data != ""){
		
                if(hexagon.returnCommand(data) == hexagon.CTRL &&  hexagon.returnOperation(data) == hexagon.GET) {

			switch(hexagon.returnParams(data)) {
				case 0:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital("sensor_data", "GLUCO", digitalRead(8)));
					} else {
						hexagon.send(hexagon.jsonMessageDigital("sensor_data", "GLUCO", digitalRead(8)));
					}
					break;
				case 1:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageAnalog("sensor_data", "TEMP", eHealth.getTemperature()));
					} else {
						hexagon.send(hexagon.jsonMessageAnalog("sensor_data", "TEMP", eHealth.getTemperature()));
					}
					break;
				case 2:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital("sensor_data", "BLOOD", eHealth.getOxygenSaturation()));
					} else {
						hexagon.send(hexagon.jsonMessageDigital("sensor_data", "BLOOD", eHealth.getOxygenSaturation()));
					}
					break;
				case 3:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital("sensor_data", "PULSE", eHealth.getBPM()));
					} else {
						hexagon.send(hexagon.jsonMessageDigital("sensor_data", "PULSE", eHealth.getBPM()));
					}
					break;
				case 4:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital("sensor_data", "AIRFLOW", eHealth.getAirFlow()));
					} else {
						hexagon.send(hexagon.jsonMessageDigital("sensor_data", "AIRFLOW", eHealth.getAirFlow()));
					}
					break;
				case 5:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageAnalog("sensor_data", "GALVANIC", eHealth.getSkinConductance()));
					} else {
						hexagon.send(hexagon.jsonMessageAnalog("sensor_data", "GALVANIC", eHealth.getSkinConductance()));
					}
					break;
				case 6:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageAnalog("sensor_data", "ECG", eHealth.getECG()));
					} else {
						hexagon.send(hexagon.jsonMessageAnalog("sensor_data", "ECG", eHealth.getECG()));
					}
					break;
				case 7:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital("sensor_data", "EMG", eHealth.getEMG()));
					} else {
						hexagon.send(hexagon.jsonMessageDigital("sensor_data", "EMG", eHealth.getEMG()));
					}
					break;
				case 8:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital("sensor_data", "POSITION", digitalRead(8)));
					} else {
						hexagon.send(hexagon.jsonMessageDigital("sensor_data", "POSITION", digitalRead(8)));
					}
					break;
			}

			delay(15);
			data = "";

		} else if(hexagon.returnCommand(data) == hexagon.STATUS &&  hexagon.returnOperation(data) == hexagon.GET)	{
			
			switch(hexagon.returnParams(data)) {
				case 0:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital("status", "BATTERY", hexagon.batteryLevel()));
					} else {
						hexagon.send(hexagon.jsonMessageDigital("status", "BATTERY", hexagon.batteryLevel()));
					}
					break;
                                case 10:
                                        if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital("status", "AVAILABLE", hexagon.availableSensors()));
					} else {
						hexagon.send(hexagon.jsonMessageDigital("status", "AVAILABLE", hexagon.availableSensors()));
					}
					break;
                                case 11:
                                        if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital("status", "CONNECTIVITY", connected));
					} else {
						hexagon.send(hexagon.jsonMessageDigital("status", "CONNECTIVITY", connected));
					}
					break;
  
			}

			delay(15);
			data = "";
		}
	}
        
        if(hexagon.batteryLevel() <= 10) {
            connected = hexagon.DISCONNECTING;
        }
	delay(1);
}

//Include always this code when using the pulsioximeter sensor
//=========================================================================
void readPulsioximeter(){  
  
  cont ++;
 
  if (cont == 50) { //Get only one of 50 measures to reduce the latency
    eHealth.readPulsioximeter();  
    cont = 0;
  }
}
