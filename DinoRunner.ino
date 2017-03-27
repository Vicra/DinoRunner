#include "SmallFS.h"
#include "MainGame.h"
#include "VGA.h"

void setup()
{
	ScreenSetup();
	Serial.begin(9600);
}

void loop()
{
	mainGameLoop();
}
