#include <Adafruit_MLX90640.h>

// 18 -> SDA
// 19 -> SCL

Adafruit_MLX90640 mlx;
float frame[32 * 24];

void setup() {
	Wire.begin(18, 19);
	Serial.begin(115200);
	delay(1200);

	Serial.println("Adafruit MLX90640 Simple Test");
	Serial.flush();
	if (mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
		Serial.println("Found Adafruit MLX90640");
	} else {
		Serial.println("MLX90640 not found!");
		while (1) delay(10);
	}


	Serial.print("Serial number: ");
	Serial.print(mlx.serialNumber[0], HEX);
	Serial.print(mlx.serialNumber[1], HEX);
	Serial.println(mlx.serialNumber[2], HEX);

	mlx.setMode(MLX90640_CHESS);
	mlx.setResolution(MLX90640_ADC_18BIT);
	mlx.setRefreshRate(MLX90640_1_HZ);
}

void loop() {
	delay(500);
	if (mlx.getFrame(frame) != 0) {
		Serial.println("Failed");
		return;
	}
	Serial.println("===================================");
	Serial.print("Ambient temperature = ");
	Serial.print(mlx.getTa(false)); // false = no new frame capture
	Serial.println(" degC");
	Serial.println();
	Serial.println();
	for (uint8_t h=0; h<24; h++) {
		for (uint8_t w=0; w<32; w++) {
			float t = frame[h*32 + w];
			Serial.print(t, 5);
			Serial.print(", ");
		}
		Serial.println();
	}
}
