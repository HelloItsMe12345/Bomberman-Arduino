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
#define wallBox 20

#define startX1 210
#define startY1 90
#define startX2 10
#define startY2 290

#define leftUnderX 10
#define leftUnderY 90
#define rightAboveX 210
#define rightAboveY 290

#define STMPE_CS 8

int x;
int y;
int xBlock = 0;
int yBlock = 80;
int spelOn = 0;
int c_knop;
int z_knop; 
int currentX = startX1;
int currentY = startY1;
int getal;
int bomX;
int bomY;
int bomDropped;

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

	while (1){	
		nunchuck_get_data();
		nunchuck_print_data();
		
		TS_Point p = ts.getPoint();
		p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
		p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
		
		if(((p.x != -8 || p.y != -11) || c_knop == 0) && spelOn == 0){
			spel();
			map();
			spelOn = 1;
		} 

		if (spelOn == 1){
			lopen();
			getal++;
		}
		
		if (bomDropped == 1){
			tft.fillRect(bomX, bomY, 20, 20, ILI9341_RED);
		}
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
	}
		
}

void lopen(){
	if (y <= -200 && (currentX + 20) < startX1){ //up
		tft.fillRect(currentX, currentY, 20, 20, ILI9341_BLACK);
		currentX = currentX + 20;
		tft.fillRect(currentX, currentY, 20, 20, ILI9341_YELLOW);
	} else if (y >= -85 && (currentX - 20) > leftUnderX){//down
		tft.fillRect(currentX, currentY, 20, 20, ILI9341_BLACK);
		currentX = currentX - 20;
		tft.fillRect(currentX, currentY, 20, 20, ILI9341_YELLOW);
	} else if (x <= 70 && (currentY - 20) > startY1){//left
		tft.fillRect(currentX, currentY, 20, 20, ILI9341_BLACK);
		currentY = currentY - 20;
		tft.fillRect(currentX, currentY, 20, 20, ILI9341_YELLOW);
	} else if (x >= 185 && (currentY + 20) < rightAboveY){//right
		tft.fillRect(currentX, currentY, 20, 20, ILI9341_BLACK);
		currentY = currentY + 20;
		tft.fillRect(currentX, currentY, 20, 20, ILI9341_YELLOW);
	}
	if (getal > 20 && bomDropped == 1){
		bomDropped = 0;
		tft.fillRect(bomX, bomY, 20, 20, ILI9341_ORANGE);
		tft.fillRect(bomX + 20, bomY, 20, 20, ILI9341_ORANGE);
		tft.fillRect(bomX - 20, bomY, 20, 20, ILI9341_ORANGE);
		tft.fillRect(bomX, bomY + 20, 20, 20, ILI9341_ORANGE);
		tft.fillRect(bomX, bomY - 20, 20, 20, ILI9341_ORANGE);
		_delay_ms(50);
		tft.fillRect(bomX, bomY, 20, 20, ILI9341_BLACK);
		tft.fillRect(bomX + 20, bomY, 20, 20, ILI9341_BLACK);
		tft.fillRect(bomX - 20, bomY, 20, 20, ILI9341_BLACK);
		tft.fillRect(bomX, bomY + 20, 20, 20, ILI9341_BLACK);
		tft.fillRect(bomX, bomY - 20, 20, 20, ILI9341_BLACK);
		
	}
	if (z_knop == 0 && getal > 20 && bomDropped == 0){
		bomX = currentX;
		bomY = currentY;
		getal = 0;
		bomDropped = 1;
	}
	_delay_ms(100);
}

void map(){
	blockFunc(190, 190);
	blockFunc(170, 170);
	blockFunc(150, 150);
	blockFunc(130, 130);
	blockFunc(190, 170);
	blockFunc(170, 130);
	blockFunc(150, 130);
	blockFunc(130, 150);	
}

void blockFunc(int xWall, int yWall){
	tft.fillRect(xWall, yWall, wallBox, wallBox, ILI9341_GREEN);
}

static void nunchuck_setpowerpins(){
	#define pwrpin PC3
	#define gndpin PC2
	DDRC |= _BV(pwrpin) | _BV(gndpin);
	PORTC &=~ _BV(gndpin);
	PORTC |= _BV(pwrpin);
	delay(100); //Stabilizeren
}

void nunchuck_init(){
	Wire.begin();
	Wire.beginTransmission(0x52);
	Wire.write(0x40);
	Wire.write(0x00);
	Wire.endTransmission();
}

void nunchuck_send_request(){
	Wire.beginTransmission(0x52);
	Wire.write(0x00);
	Wire.endTransmission();
}

int nunchuck_get_data(){
	int cnt=0;
	Wire.requestFrom (0x52, 6);

	while (Wire.available ()){
		nunchuck_buf[cnt] = nunchuk_decode_byte(Wire.read());
		cnt++;
	}

	nunchuck_send_request();

	if (cnt >= 5){
		return 1;
	}

	return 0;
}

void nunchuck_print_data(){
	int z_button = 0;
	int c_button = 0;

	if ((nunchuck_buf[5] >> 0) & 1)
		z_button = 1;

	if ((nunchuck_buf[5] >> 1) & 1)
		c_button = 1;
	
	int joy_x_axis = nunchuck_buf[0];
	int joy_y_axis = nunchuck_buf[1];

	x = joy_x_axis;
	y = ~joy_y_axis;
	c_knop = c_button;
	z_knop = z_button;
}

char nunchuk_decode_byte (char x){
	x = (x ^ 0x17) + 0x17;
	return x;
}

int nunchuck_zbutton(){
	return ((nunchuck_buf[5] >> 0) & 1) ? 0 : 1;
}

int nunchuck_cbutton(){
	return ((nunchuck_buf[5] >> 1) & 1) ? 0 : 1;
}

int nunchuck_joyx(){
	return nunchuck_buf[0];
}

int nunchuck_joyy(){
	return nunchuck_buf[1];
}
