**ESP8266 MQTT  For Honeywell IPM Air Quality Sensors**

* Refer to the pinout diagram for your board to determine SDA and SCL pins. For WemosD1Mini the pins are D1 (SCL) and D2 (SDA)
* Honeywell IPM requies 5V which I got directly from the WemosD1Mini.
* Pin 3 on the sensor needs to be connected to GND for I2C mode
* The four connections are SCL, SDA, 5V and GND
* Edit the Wifi SSID, Wifi Password, MQTT Server Address, MQTT Username and MQTT Password in mqtt.ino
* Upload the sketch.
* To troubleshoot open serial monitor at 115200bps to check status/errors.
