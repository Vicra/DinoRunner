#include "VGA.h"
#include "Sprites.h"

void ScreenSetup()
{
	VGA.begin(VGAWISHBONESLOT(9), CHARMAPWISHBONESLOT(10));
	VGA.clear();
	VGA.setBackgroundColor(BLACK);
}

void drawPlayer(){
	if (IsJumping)
	{
		player_layer_count = 2;
	}
	else if (IsDucking)
	{
		player_layer_count = 0;
	}
	else
	{
		player_layer_count++;
		if(player_layer_count>=2)
        	player_layer_count = 0;
	}
	VGA.setColor(BLACK);
    VGA.clearArea(player_lastKnown_posX,player_lastKnown_posY,player_width,player_height);  
    VGA.writeArea(player_posX,player_posY,player_width,player_height,player_layers[player_layer_count]);
}

void drawGroundLine(){
	VGA.writeArea(0,105,160,1,ground);// posx, posy, width, height, sprite
}

void drawDots(){
	
}

void drawCactus(){
	VGA.setColor(BLACK);
    VGA.clearArea(cactus_lastKnown_posX,cactus_lastKnown_posY,cactus_width,cactus_height);  
    VGA.writeArea(cactus_posX,cactus_posY,cactus_width,cactus_height,cactusPixel);
}