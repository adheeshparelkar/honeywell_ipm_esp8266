**ESP8266 MQTT  For Honeywell IPM Air Quality Sensors**

*  Refer to the pinout diagram for your board to determine SDA and SCL pins. For WemosD1Mini the pins are D1 (SCL) and D2 (SDA)
*  Honeywell IPM requies 5V which I got directly from the WemosD1Mini.
    *     Pin 1: 5V (Red)
    *     Pin 2: GND (Black)
    *     Pin3: GND (Yellow)
    *     Pin4: SDA (White)
    *     Pin 5: SCL (Yellow)

*  Edit the placeholders for Wifi SSID, Wifi Password, MQTT Server Address, MQTT Username and MQTT Password.
*  Upload the sketch.
*  To troubleshoot open serial monitor at 115200bps to check status/errors.
