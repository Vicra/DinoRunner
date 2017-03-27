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
	else if (IsRunning)
	{
		if (player_layer_count == 4)
			player_layer_count = 5;
		else
			player_layer_count = 4;
	}
	else
	{
		player_layer_count++;
		if (player_layer_count >= 2)
			player_layer_count = 0;
	}
	if (IsDead)
	{
		player_layer_count = 3;
	}
	VGA.setColor(BLACK);
	VGA.clearArea(player_lastKnown_posX, player_lastKnown_posY, player_width, player_height);
	VGA.clearArea(player_lastKnown_posX, player_lastKnown_posY, 29, 11);
	if(IsRunning){
		VGA.writeArea(player_posX, 90, 29, 11, player_layers[player_layer_count]);
	}
	else{
		VGA.clearArea(player_lastKnown_posX, 90, 29, 11);
		if (IsDead)
			VGA.clearArea(cactus_lastKnown_posX, cactus_lastKnown_posY, cactus_width, cactus_height);
		VGA.writeArea(player_posX, player_posY, player_width, player_height, player_layers[player_layer_count]);
	}
}

void drawGroundLine() {
	VGA.writeArea(0, 105, 160, 1, ground); // posx, posy, width, height, sprite
}

void drawDots() {
	VGA.setColor(BLACK);
	dust_x -= cactus_velocity;
	dust_x2 -= cactus_velocity;
	dust_x3 -= cactus_velocity;
	dust_x4 -= cactus_velocity;
	dust_x5 -= cactus_velocity;
	dust_x6 -= cactus_velocity;
	dust_x7 -= cactus_velocity;
	dust_x8 -= cactus_velocity;
	dust_x9 -= cactus_velocity;

	VGA.clearArea(dust_lastKnown_x, dust_lastKnown_y, 1, 1);
	VGA.writeArea(dust_x, dust_y, 1, 1, dust1);

	VGA.clearArea(dust_lastKnown_x2, dust_lastKnown_y2, 1, 1);
	VGA.writeArea(dust_x2, dust_y2, 1, 1, dust1);

	VGA.clearArea(dust_lastKnown_x3, dust_lastKnown_y3, 1, 1);
	VGA.writeArea(dust_x3, dust_y3, 1, 1, dust1);

	VGA.clearArea(dust_lastKnown_x4, dust_lastKnown_y4, 1, 1);
	VGA.writeArea(dust_x4, dust_y4, 1, 1, dust1);

	VGA.clearArea(dust_lastKnown_x5, dust_lastKnown_y5, 1, 1);
	VGA.writeArea(dust_x5, dust_y5, 1, 1, dust1);

	VGA.clearArea(dust_lastKnown_x6, dust_lastKnown_y6, 1, 1);
	VGA.writeArea(dust_x6, dust_y6, 1, 1, dust1);

	VGA.clearArea(dust_lastKnown_x7, dust_lastKnown_y7, 1, 1);
	VGA.writeArea(dust_x7, dust_y7, 1, 1, dust1);

	VGA.clearArea(dust_lastKnown_x8, dust_lastKnown_y8, 1, 1);
	VGA.writeArea(dust_x8, dust_y8, 1, 1, dust1);

	VGA.clearArea(dust_lastKnown_x9, dust_lastKnown_y9, 1, 1);
	VGA.writeArea(dust_x9, dust_y9, 1, 1, dust1);
}

void drawCactus() {
	VGA.setColor(BLACK);
	VGA.clearArea(cactus_lastKnown_posX, cactus_lastKnown_posY, cactus_width, cactus_height);
	if (!ActiveCactus)
		VGA.writeArea(cactus_posX, cactus_posY, cactus_width, cactus_height, cactusPixel[3]);
	else
		VGA.writeArea(cactus_posX, cactus_posY, cactus_width, cactus_height, cactusPixel[randomCactus]);
}

void drawClouds(){
	VGA.setColor(BLACK);

	VGA.clearArea(cloud_lastKnown_x, cloud_lastKnown_y, cloud_width, cloud_height);
	VGA.writeArea(cloud_x, cloud_y, cloud_width, cloud_height, cloudPixel1);

	VGA.clearArea(cloud2_lastKnown_x, cloud2_lastKnown_y, cloud2_width, cloud2_height);
	VGA.writeArea(cloud2_x, cloud2_y, cloud2_width, cloud2_height, cloudPixel2);	

	VGA.clearArea(cloud3_lastKnown_x, cloud3_lastKnown_y, cloud3_width, cloud3_height);
	VGA.writeArea(cloud3_x, cloud3_y, cloud3_width, cloud3_height, cloudPixel3);	
}