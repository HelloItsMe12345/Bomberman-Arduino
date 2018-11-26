#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_STMPE610.h>
#include <avr/io.h>

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

#define TS_MINX 130
#define TS_MINY 130
#define TS_MAXX 4000
#define TS_MAXY 3800

#define BoxX 10
#define BoxY 10

#define STMPE_CS 8

int x;
int y;
int xBlock = 0;
int yBlock = 80;
int spelOn = 0;
int c_knop; 

static uint8_t nunchuck_buf[6]; //Data Nunchuk opslaan

Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


int main(void)
{
	init();
	
	nunchuck_setpowerpins();
	nunchuck_init();
		
	Serial.begin(9600);
	Serial.println("ILI9341 Test!");

	if (!ts.begin()) {
		Serial.println("Couldn't start touchscreen controller");
		while (1);
	}
	tft.begin();
	beginScherm();
	sei();

	while (1) //infinite loop
	{	
		nunchuck_get_data();
		nunchuck_print_data();
		
		TS_Point p = ts.getPoint();
		p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
		p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
		
		if((p.x != -8 || p.y != -11) && spelOn == 0){
			spel();
			spelOn = 1;
		} 
		
		//cIngedrukt();
// 		Serial.print(p.x);
// 		Serial.print("\t");
// 		Serial.println(p.y);
	}
}

void beginScherm(){
	tft.fillScreen(ILI9341_BLACK);
	tft.setRotation(1);
	tft.setTextSize(5);
	tft.setCursor(30, 75);
	tft.print("Bomberman");
	tft.setRotation(0);
		
	tft.setRotation(1);
	tft.setTextSize(1);
	tft.setCursor(80, 180);
	tft.print("Press screen or 'c' to start ");
	tft.setRotation(0);
}

void spel(){		
	tft.fillScreen(ILI9341_BLACK);
	tft.setRotation(1);
	tft.setTextSize(5);
	tft.setCursor(150, 100);
	tft.print("Spel");
	
	tft.setTextSize(1);
	tft.setCursor(10,10);
	tft.print("Tijd:");
	tft.setTextSize(2);
	tft.setCursor(10,35);
	tft.print("03:00");
	
	tft.setTextSize(1);
	tft.setCursor(10,70);
	tft.print("speler 1:");
	tft.setTextSize(2);
	tft.setCursor(10,100);
	tft.print("2");
	tft.setCursor(30,100);
	tft.print("x");
	
	tft.setTextSize(1);
	tft.setCursor(10,140);
	tft.print("speler 2:");
	tft.setTextSize(2);
	tft.setCursor(10,170);
	tft.print("0");
	tft.setCursor(30,170);
	tft.print("x");
	tft.setRotation(0);
	
	for (int i = 0; i < 4; i++){
		for (int up = 0; up < 23; up++){
			xBlock = xBlock + 10;
			tft.fillRect(xBlock,yBlock,BoxX,BoxY,ILI9341_WHITE);
		}			
		for (int right = 0; right < 23; right++){
			yBlock = yBlock + 10;
			tft.fillRect(xBlock,yBlock,BoxX,BoxY,ILI9341_WHITE);
		}
		for (int down = 0; down < 23; down++){
			xBlock = xBlock - 10;
			tft.fillRect(xBlock,yBlock,BoxX,BoxY,ILI9341_WHITE);
		}
		for (int left = 0; left < 23; left++){
			yBlock = yBlock - 10;
			tft.fillRect(xBlock,yBlock,BoxX,BoxY,ILI9341_WHITE);
		}
		
		/*tft.fillRect(i, i, BoxX, BoxY, ILI9341_WHITE);*/
		
	}
	
}

void cIngedrukt()
{
	if (c_knop == 0 && spelOn == 0)
	{
		spel();
		spelOn = 1;
	}
}

static void nunchuck_setpowerpins()
{
	#define pwrpin PC3
	#define gndpin PC2
	DDRC |= _BV(pwrpin) | _BV(gndpin);
	PORTC &=~ _BV(gndpin);
	PORTC |= _BV(pwrpin);
	delay(100); //Stabilizeren
}

void nunchuck_init()
{
	Wire.begin();
	Wire.beginTransmission(0x52);
	Wire.write(0x40);
	Wire.write(0x00);
	Wire.endTransmission();
}

void nunchuck_send_request()
{
	Wire.beginTransmission(0x52);
	Wire.write(0x00);
	Wire.endTransmission();
}

int nunchuck_get_data()
{
	int cnt=0;
	Wire.requestFrom (0x52, 6);

	while (Wire.available ())
	{
		nunchuck_buf[cnt] = nunchuk_decode_byte(Wire.read());
		cnt++;
	}

	nunchuck_send_request();

	if (cnt >= 5)
	{
		return 1;
	}

	return 0;
}

void nunchuck_print_data()
{
	int c_button = 0;

	if ((nunchuck_buf[5] >> 1) & 1)
	c_button = 1;	

	c_knop = c_button;
}

char nunchuk_decode_byte (char x)
{
	x = (x ^ 0x17) + 0x17;
	return x;
}

int nunchuck_cbutton()   	 //Returnt status van de C-knop
{
	return ((nunchuck_buf[5] >> 1) & 1) ? 0 : 1;
}
