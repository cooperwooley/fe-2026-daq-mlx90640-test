#include <Wire.h>

void setup() {
	Wire.begin(18, 19);
	Serial.begin(115200);
  delay(1000);

	Serial.println("I2C Scanner");
	
	for (uint8_t addr = 1; addr < 127; addr++) {
		Wire.beginTransmission(addr);
		if (Wire.endTransmission() == 0) {
			Serial.print("Found I2C device at 0x");
			Serial.println(addr, HEX);
			delay(10);
		}
	}
}

void loop() {}
