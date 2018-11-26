

void cIngedrukt(int c_knop, int spelOn)   	 //Extraatje zodat het scherm gerefresht wordt
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