#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <avr/io.h>

//Define pin 3
#define BL_FULL		PORTD |= (1 << PORTD3)
#define BL_OFF		PORTD &= ~(1 << PORTD3)
#define BL_TOGGLE	PIND |= (1 << PIND3)


//Define LCD
#define TFT_DC 9
#define TFT_CS 10
#define STMPE_CS 8

Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int main(void)
{
	init();
	
	DDRD |= (1 << DDD3); 
	

	//Setup LCD
	tft.begin();
	tft.fillScreen(ILI9341_RED);
	BL_FULL;
	
	while(1)
	{
	}
}