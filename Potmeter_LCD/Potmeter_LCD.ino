#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <avr/io.h>

#define TFT_DC 9
#define TFT_CS 10
#define STMPE_CS 8

Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int main(void)
{
	init();

	//Scherm setup:
	tft.begin();
	tft.fillScreen(ILI9341_WHITE);
	
	while(1)
	{

	}
}