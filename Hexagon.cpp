/**
 * Hexagon Framework
 */

#include "Hexagon.h"
#include "stdlib.h"
#include <eHealth.h>


/**
 * Constructor
 */
hexagonClass::hexagonClass(void) {}

/**
 * [hexagonClass::initWireless Initialise the bluetooth communication between Arduino and Android]
 * @param name [Friednly name for the Arduino]
 * @param pin  [pin number for the Bluetooth communication]
 */
void hexagonClass::initBluetooth(char access_point_name[], unsigned long p) {
	
	char pin[6];	// Five digits pin plus a null terminator
	char name[30];	// 29 chars for the naming command, 17 chars for the actual name
	char auth[20];	// 19 chars for the pin auth command, 5 chars for the actual pin

	
	Serial.begin(115200);	// Start the serial channel for debuging
	Serial.flush();			// Flush the channel before print anything

	ltoa(p, pin, 10);					// Convert long int to string
	strcpy(name, "SET BT NAME ");		// 17 chars available for name after this
	strcat(name, access_point_name);
	strcpy(auth, "SET BT AUTH * ");		// 5 chars available for pin after this
	strcat(auth, pin);

	Serial.println("SET BT PAGEMODE 3 2000 1");		// If valid pin then setup the bluetooth module 
  	Serial.println(name);							// Serial.println("SET BT NAME " + access_point_name);
	Serial.println("SET BT ROLE 0 f 7d00");
	Serial.println("SET CONTROL ECHO 0");
	Serial.println(auth);							// Serial.println("SET BT AUTH * " + pin);
	Serial.println("SET CONTROL ESCAPE - 00 1");
	Serial.println("SET CONTROL BAUD 115200,8n1");
	
	Serial.flush();
	delay(10);
}

/**
 * [hexagonClass::initWifi description]
 * @param access_point_name [description]
 */
void hexagonClass::initWifi(char access_point_name[]) {
	
	char name[30];
	
	strcpy(name, "join ");
	strcat(name, access_point_name);
	strcat(name, "\r");

	Serial.begin(9600);
	Serial.flush();
	
	delay(5);
	
	Serial.print("$$$"); 
	hexagon.check();
  	
  	// Sets DHCP and TCP protocol
    Serial.print("set ip dhcp 1\r");
    hexagon.check();
    Serial.print("set ip protocol 1\r");
    hexagon.check();
  	
  	// Configures the way to join the network AP
    Serial.print("set wlan join 0\r");
    hexagon.check();
    Serial.print(name);
    hexagon.check();
  
    Serial.print("set i h 255.255.255.255\r");
    delay(1000);
    
    Serial.print("set i r 12345\r");
    hexagon.check();
    Serial.print("set i l 2000\r");
    hexagon.check();
    Serial.print("exit\r");
    hexagon.check();
	
	Serial.flush();
}

/**
 * [hexagonClass::check description]
 */
void hexagonClass::check() {
	
	uint8_t cont = 0;
	char recv[128];
	
	delay(500);
  	
  	while (Serial.available()>0) {
    	recv[cont]=Serial.read();
    	delay(10);
    	cont++;
  	}
  
	recv[cont]='\0';
	Serial.println(recv);
	Serial.flush();
	delay(100);
}

/**
 * [hexagonClass::dataAvailable Available data on the wireless link]
 * @return [length of available data]
 */
int hexagonClass::dataAvailable() { //maybe I should change this to boolean??
	
	// if(Serial.available() > 0) {
	// 	return true;
	// } else {
	// 	return false;
	// }
	return Serial.available();
}

/**
 * [hexagonClass::xmlMessage Encapulate message to xml container]
 * @param  type   []
 * @param  sensor [description]
 * @param  value  [description]
 * @return        [description]
 */
String hexagonClass::xmlMessageAnalog(String type, String sensor, float value) {

	char s[10];
	dtostrf(value, 2, 3, s);
	String val = String(s);
	String mssg;
	
	mssg = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	delay(1);
	mssg.concat("<data>");
	delay(1);
	mssg.concat("<message_type>");
	delay(1);
	mssg.concat(type);
	delay(1);
	mssg.concat("</message_type>");
	delay(1);
	mssg.concat("<type>");
	delay(1);
	mssg.concat(sensor);
	delay(1);
	mssg.concat("</type>");
	delay(1);
	mssg.concat("<value>");
	delay(1);
	mssg.concat(val);
	delay(1);
	mssg.concat("</value>");
	delay(1);
	mssg.concat("</data>");
	delay(1);
	
	return mssg;

}

String hexagonClass::xmlMessageDigital(String type, String sensor, unsigned long value) {

	String val = String(value);
	String mssg;
	
	mssg = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	delay(1);
	mssg.concat("<data>");
	delay(1);
	mssg.concat("<message_type>");
	delay(1);
	mssg.concat(type);
	delay(1);
	mssg.concat("</message_type>");
	delay(1);
	mssg.concat("<type>");
	delay(1);
	mssg.concat(sensor);
	delay(1);
	mssg.concat("</type>");
	delay(1);
	mssg.concat("<value>");
	delay(1);
	mssg.concat(val);
	delay(1);
	mssg.concat("</value>");
	delay(1);
	mssg.concat("</data>");
	delay(1);
	
	return mssg;

}

/**
 * [hexagonClass::jsonMessage Encapulate message to json container]
 * @param  type   [description]
 * @param  sensor [description]
 * @param  value  [description]
 * @return        [description]
 */
String hexagonClass::jsonMessageDigital(String type, String sensor, unsigned long value) {

	String val = String(value);
	String mssg;
	
	mssg = "{\"data\":[";
	delay(1);
	mssg.concat("{");
	delay(1);
	mssg.concat("\"message_type\":\"");
	delay(1);
	mssg.concat(type);
	delay(1);
	mssg.concat("\",");
	delay(1);
	mssg.concat("\"type\":\"");
	delay(1);
	mssg.concat(sensor);
	delay(1);
	mssg.concat("\",");
	delay(1);
	mssg.concat("\"value\":\"");
	delay(1);
	mssg.concat(val);
	delay(1);
	mssg.concat("\"");
	delay(1);
	mssg.concat("}");
	delay(1);
	mssg.concat("]}");
	delay(1);
	
	return mssg;
}

String hexagonClass::jsonMessageAnalog(String type, String sensor, float value) {

	char s[10];
	dtostrf(value, 2, 3, s);
	String val = String(s);
	String mssg;
	
	mssg = "{\"data\":[";
	delay(1);
	mssg.concat("{");
	delay(1);
	mssg.concat("\"message_type\":\"");
	delay(1);
	mssg.concat(type);
	delay(1);
	mssg.concat("\",");
	delay(1);
	mssg.concat("\"type\":\"");
	delay(1);
	mssg.concat(sensor);
	delay(1);
	mssg.concat("\",");
	delay(1);
	mssg.concat("\"value\":\"");
	delay(1);
	mssg.concat(val);
	delay(1);
	mssg.concat("\"");
	delay(1);
	mssg.concat("}");
	delay(1);
	mssg.concat("]}");
	delay(1);
	
	return mssg;
}

/**
 * [hexagonClass::send Sends data to the wireless chanel]
 * @param msg [description]
 */
void hexagonClass::send(String msg) {
	Serial.println(msg);
}

/**
 * [hexagonClass::returnCommand Retruns the type of the command received from Android]
 * @param  data [description]
 * @return      [description]
 */
int hexagonClass::returnCommand(String data) {

	char *command = splitter(data, 1);
	
	if(strcmp(command,"0") == 0) {
		return CTRL;	
	} else if(strcmp(command,"1") == 0) {
		return STATUS;
	} else {
		return -1;
	}

}

/**
 * [hexagonClass::returnOperation Retruns the type of the operation received from Android]
 * @param  data [description]
 * @return      [description]
 */
int hexagonClass::returnOperation(String data) {

	char *command = splitter(data, 2);

	if(strcmp(command,"0") == 0) {
		return GET;	
	} else {
		return -1;
	}
}

/**
 * [hexagonClass::returnParams Retruns params related to the command and operation received from Android]
 * @param  data [description]
 * @return      [description]
 */
int hexagonClass::returnParams(String data) {

	char *command = splitter(data, 3);

	if(strcmp(command,"0") == 0) {
		return GLUCO;	
	}else if(strcmp(command,"1") == 0) {
		return TEMP;
	}else if(strcmp(command,"2") == 0){
		return BLOOD;
	}else if(strcmp(command,"3") == 0){
		return PULSE;
	}else if(strcmp(command,"4") == 0){
		return AIRFLOW;
	}else if(strcmp(command,"5") == 0){
		return GALVANIC;
	}else if(strcmp(command,"6") == 0){
		return ECG;
	}else if(strcmp(command,"7") == 0){
		return EMG;
	}else if(strcmp(command,"8") == 0){
		return POSITION;
	}else if(strcmp(command,"9") == 0){
		return BATTERYLEVEL;
	}else if(strcmp(command,"10") == 0){
		return AVAILABLE;
	}else if(strcmp(command,"11") == 0){
		return CONNECTIVITY;
	}else {
		return -1;
	}
}

int hexagonClass::returnContainer(String data) {

	char *command = splitter(data, 4);

	if(strcmp(command,"0") == 0) {
		return XMLCONTAINER;	
	}else if(strcmp(command,"1") == 0) {
		return JSONCONTAINER;
	}else {
		return -1;
	}
}

char* hexagonClass::splitter(String message, int token) {
	
	char *token1;
	char *token2;
	char *token3;
	char *token4;
	char cdata[65];
	strncpy(cdata, message.c_str(), sizeof(cdata));
	cdata[sizeof(cdata) - 1] = 0;

	token1 = strtok(cdata, ";");
	token2 = strtok(NULL, ";");
	token3 = strtok(NULL, ";");
	token4 = strtok(NULL, ";");
	
	switch(token) {
		case 1:
			return token1;
			break;
		case 2:
			return token2;
			break;
		case 3:
			return token3;
			break;
		case 4:
			return token4;
			break;
	}
}


long hexagonClass::readVcc() {
	
	long result;
	// Read 1.1V reference against AVcc
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	delay(2); // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Convert
	while (bit_is_set(ADCSRA,ADSC));
	result = ADCL;
	result |= ADCH<<8;
	result = 1125300L / result; // Back-calculate AVcc in mV
	return result;
}

long hexagonClass::batteryLevel() {
	double voltage = hexagon.readVcc()/1000.0;
	long level = (voltage * 100)/5.04;
	
	if(voltage >= 5.04) {
		return 100;
	} else {
		return level;	
	}
}

unsigned long hexagonClass::availableSensors() {
	
	char availableSensors[10] = {'0','0','0','0','0','0','0','0','0','\0'};
	

	unsigned long i;
	
	if(hexagon.checkSensor(0) > 0){
		availableSensors[0] = '1';	
	}
	if(hexagon.checkSensor(1) > 0) {
		availableSensors[1] = '1';		
	}
	if(hexagon.checkSensor(2) > 0) {
		availableSensors[2] = '1';		
	}
	if(hexagon.checkSensor(3) > 0) {
		availableSensors[3] = '1';		
	}
	if(hexagon.checkSensor(4) > 0) {
		availableSensors[4] = '1';
	}
	if(hexagon.checkSensor(5) > 0) {
		availableSensors[5] = '1';
	}
	if(hexagon.checkSensor(6) > 0) {
		availableSensors[6] = '1';
	}
	if(hexagon.checkSensor(7) > 0) {
		availableSensors[7] = '1';
	}
	if(hexagon.checkSensor(8) > 0) {
		availableSensors[8] = '1';
	}

	i = strtoul (availableSensors, NULL, 10);
	return i;

}

int hexagonClass::checkSensor(int pin) {
	
	int sum = 0;

	switch(pin) {
		case 0:
			for(int i = 0; i < 10; i++) {
				sum += digitalRead(8);
				delay(3);
			}
			break;
		case 1:
			for(int i = 0; i < 10; i++) {
				sum += int(eHealth.getTemperature());
				delay(3);
			}
			break;
		case 2:
			for(int i = 0; i < 10; i++) {
				sum += digitalRead(10);
				delay(3);
			}
			break;
		case 3:
			for(int i = 0; i < 10; i++) {
				sum += digitalRead(9);
				delay(3);
			}
			break;
		case 4:
			for(int i = 0; i < 10; i++) {
				sum += int(eHealth.getAirFlow());
				delay(3);
			}
			break;
		case 5:
			for(int i = 0; i < 10; i++) {
				sum += int(eHealth.getSkinConductance());
				delay(3);
			}
			break;
		case 6:
			for(int i = 0; i < 10; i++) {
				sum += int(eHealth.getECG());
				delay(3);
			}
			break;
		case 7:
			for(int i = 0; i < 10; i++) {
				sum += int(eHealth.getEMG());
				delay(3);
			}
			break;
		case 8:
			for(int i = 0; i < 10; i++) {
				sum += digitalRead(8);
				delay(3);
			}
			break;
	}
	
	return sum;
}
hexagonClass hexagon = hexagonClass();