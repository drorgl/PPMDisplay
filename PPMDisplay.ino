#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "Timer1.h"
#include "PPMIn.h"
#include "inputchannel.h"

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 5, 7, 6);

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
// Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

rc::PPMIn ppm_decoder;


//If you wish to increase this value, you'll need to change the locations as well
#define NUMBER_CHANNELS 8

const int8_t  locations[] = {
	0,0,
	0,10,
	0,20,
	0,30,
	48,0,
	48,10,
	48,20,
	48,30
};

void setup() {
	rc::Timer1::init();
	display.begin();

	display.setContrast(40);

	display.clearDisplay();   // clears the screen and buffer


	ppm_decoder.setPin(3);
	ppm_decoder.setPauseLength(3000);
	ppm_decoder.setTimeout(500);
	ppm_decoder.start();
	
}


void loop() {
	ppm_decoder.update();
	
	display.clearDisplay();
	
	if (!ppm_decoder.isLost()) {
		display.setTextSize(0);
		for (auto i = 0; i < NUMBER_CHANNELS; i++) {
			display.setCursor(locations[i * 2], locations[(i * 2) + 1]);
			display.print(rc::getInputChannel((rc::InputChannel)i));
		}
	}
	else {
		display.setCursor(0, 0);
		display.setTextSize(2);
		display.println("SIGNAL");
		display.println("LOST");
	}

	if (!ppm_decoder.isStable()) {
		display.setTextSize(0);
		display.setCursor(0, 40);
		display.println("Sgnl Unstable");
	}

	display.display();
	delay(200);
}
