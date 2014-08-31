#include <PinChangeInt.h>
#include <eHealth.h>


#include <Hexagon.h>

String data;
char character;
int connected;
uint8_t cont = 0;

void setup() {
	hexagon.initWifi("Alex", "12345");
        eHealth.initPulsioximeter();
        
        PCintPort::attachInterrupt(6, readPulsioximeter, RISING);
        delay(1000);
        connected = hexagon.CONNECTED;
        Serial.flush();
}

void loop() {

        while(hexagon.dataAvailable() > 0){
                character = Serial.read();
                data.concat(character);	
                delay(1);
        }

	if(data != ""){
		
                if(hexagon.returnCommand(data) == hexagon.CTRL &&  hexagon.returnOperation(data) == hexagon.GET) {

			switch(hexagon.returnParams(data)) {
				case 0:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital(hexagon.SENSORDATA, hexagon.GLUCO, digitalRead(8)));
					} else {
						hexagon.send(hexagon.jsonMessageDigital(hexagon.SENSORDATA, hexagon.GLUCO, digitalRead(8)));
					}
					break;
				case 1:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageAnalog(hexagon.SENSORDATA, hexagon.TEMP, eHealth.getTemperature() * hexagon.accuFix()));
					} else {
						hexagon.send(hexagon.jsonMessageAnalog(hexagon.SENSORDATA, hexagon.TEMP, eHealth.getTemperature() * hexagon.accuFix()));
					}
					break;
				case 2:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital(hexagon.SENSORDATA, hexagon.BLOOD, eHealth.getOxygenSaturation()));
					} else {
						hexagon.send(hexagon.jsonMessageDigital(hexagon.SENSORDATA, hexagon.BLOOD, eHealth.getOxygenSaturation()));
					}
					break;
				case 3:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital(hexagon.SENSORDATA, hexagon.PULSE, eHealth.getBPM()));
					} else {
						hexagon.send(hexagon.jsonMessageDigital(hexagon.SENSORDATA, hexagon.PULSE, eHealth.getBPM()));
					}
					break;
				case 4:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital(hexagon.SENSORDATA, hexagon.AIRFLOW, eHealth.getAirFlow()));
					} else {
						hexagon.send(hexagon.jsonMessageDigital(hexagon.SENSORDATA, hexagon.AIRFLOW, eHealth.getAirFlow()));
					}
					break;
				case 5:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageAnalog(hexagon.SENSORDATA, hexagon.GALVANIC, eHealth.getSkinConductance() * hexagon.accuFix()));
					} else {
						hexagon.send(hexagon.jsonMessageAnalog(hexagon.SENSORDATA, hexagon.GALVANIC, eHealth.getSkinConductance() * hexagon.accuFix()));
					}
					break;
				case 6:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageAnalog(hexagon.SENSORDATA, hexagon.ECG, eHealth.getECG() * hexagon.accuFix()));
					} else {
						hexagon.send(hexagon.jsonMessageAnalog(hexagon.SENSORDATA, hexagon.ECG, eHealth.getECG() * hexagon.accuFix()));
					}
					break;
				case 7:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital(hexagon.SENSORDATA, hexagon.EMG, eHealth.getEMG()));
					} else {
						hexagon.send(hexagon.jsonMessageDigital(hexagon.SENSORDATA, hexagon.EMG, eHealth.getEMG()));
					}
					break;
				case 8:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital(hexagon.SENSORDATA, hexagon.POSITION, digitalRead(8)));
					} else {
						hexagon.send(hexagon.jsonMessageDigital(hexagon.SENSORDATA, hexagon.POSITION, digitalRead(8)));
					}
					break;
			}

			delay(15);
			data = "";

		} else if(hexagon.returnCommand(data) == hexagon.STATUS &&  hexagon.returnOperation(data) == hexagon.GET)	{
			
			switch(hexagon.returnParams(data)) {
				case 0:
					if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital(hexagon.STATUSDATA, hexagon.BATTERYLEVEL, hexagon.batteryLevel()));
					} else {
						hexagon.send(hexagon.jsonMessageDigital(hexagon.STATUSDATA, hexagon.BATTERYLEVEL, hexagon.batteryLevel()));
					}
					break;
                                case 10:
                                        if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital(hexagon.STATUSDATA, hexagon.AVAILABLE, hexagon.availableSensors()));
					} else {
						hexagon.send(hexagon.jsonMessageDigital(hexagon.STATUSDATA, hexagon.AVAILABLE, hexagon.availableSensors()));
					}
					break;
                                case 11:
                                        if(hexagon.returnContainer(data) == hexagon.XMLCONTAINER) {
						hexagon.send(hexagon.xmlMessageDigital(hexagon.STATUSDATA, hexagon.CONNECTIVITY, connected));
					} else {
						hexagon.send(hexagon.jsonMessageDigital(hexagon.STATUSDATA, hexagon.CONNECTIVITY, connected));
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
        data = "";
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
