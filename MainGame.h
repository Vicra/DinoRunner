#include "Screen.h"
#include "VGA.h"
#include <stdlib.h>
#include "VarInit.h"
#include "InputManager.h"

void mainGameLoop() {
	switch (gameState) {
		case MENU_STATE:
			menu();
			break;
		case ENDLESS_STATE:
			endless();
			break;
		case GAMEOVER_STATE:
			gameOver();
			break;
		case SCORES_STATE:
			scores();
			break;
		case INSTRUCTIONS_STATE:
			instructions();
			break;
	}
}

void playerJump() {
	if (IsJumping)
	{
		if (player_posY <= 40) { // as higher as he can get
			landing = true;
		}

		if (landing) {
			if (IsDropping) {
				down_velocity = defaultFasterVelocity;
			}
			else {
				down_velocity = defaultVelocity;
			}
			player_posY += down_velocity;
		}
		else {
			player_posY -= 15;
		}

		if (player_posY >= 80) { // on ground position
			player_posY = 80;
			IsJumping = false;
			IsDropping = false;
			landing = false;
			down_velocity = defaultVelocity;
		}
	}
}

void processEvent(enum event_t ev) {
	IsRunning = false;
	if (ev == event_1) {
		if (!IsJumping) {
			IsJumping = true;
		}
	}
	else if (ev == event_2) {
		if (IsJumping) {
			IsDropping = true;
		}
	}
	else if (ev == event_3) {
		if (!IsJumping && !IsDropping) {
			IsRunning = true;
			speedMult = 2;
		}
	}
}

void printScoreLabel() {
	VGA.setColor(BLUE);
	VGA.printtext(70, 0, "Score:");
	VGA.setColor(WHITE);
}

void checkPause()
{
  if(!digitalRead(FPGA_SW_0))
  {
    while(true)
    {
      VGA.setColor(227, 38, 54);
      VGA.printtext(60,45,"PAUSED");
      VGA.printtext(20,60,"BUTTON 4 TO EXIT");
      
      //salir de pausa
      if(digitalRead(FPGA_SW_0))
      {
      	VGA.clear();
		printScoreLabel();
		drawGroundLine();
        break;
      }
      //regresar al menu
      if(digitalRead(FPGA_BTN_3))
      {
        vidas = 0;
        return;
      }
      delay(100);
    }
  }
}

void menu() {
	VGA.setColor(GREEN);
	VGA.printtext(55, 25, "Dinno");
	VGA.printtext(50, 40, "Runner!");
	VGA.setColor(PURPLE);
	VGA.printtext(35, 53, "1.Start Game");
	VGA.setColor(WHITE);
	VGA.printtext(35, 66, "2.Scores");
	VGA.printtext(35, 79, "3.Instructions");
	checkInputFromMenu();
}

void drawHearts() {
	int heartWidth = 10;
	int heartHeight = 8;
	int hearty = 0;

	int heart1x = 10;

	int heart2x = 25;

	int heart3x = 40;

	VGA.setColor(BLACK);

	VGA.clearArea(heart1x, hearty, heartWidth, heartHeight);
	VGA.clearArea(heart2x, hearty, heartWidth, heartHeight);
	VGA.clearArea(heart3x, hearty, heartWidth, heartHeight);

	if (vidas > 0) {
		VGA.writeArea(heart1x, hearty, heartWidth, heartHeight, heartcontainer);
		if (vidas > 1) {
			VGA.writeArea(heart2x, hearty, heartWidth, heartHeight, heartcontainer);
			if (vidas > 2) {
				VGA.writeArea(heart3x, hearty, heartWidth, heartHeight, heartcontainer);
			}
		}
	}
}

void cactusAppear() {
	if (!ActiveCactus){
		int cactusGen = rand() % 2;
		if (cactusGen == 1){
			ActiveCactus = true;
		}
	}
}

void printScoreOnScreen() {
	char sc [4];
	itoa(score, sc, 10);
	VGA.setColor(WHITE);
	VGA.printtext(120, 0, sc);
}

void globalReset(){
	IsDead = false;
	vidas = 3;
	score = 0;
	cactus_velocity = 6;
	speedMult = 1;
	timerCounter = 0;
	scoreTimeCount = 0;
	cactusTimeCount = 0;
	cactusSpawnRate = 0;
	cactus_posX = 145;
	cactus_posY = Screen_height + (cactus_height/5);
	ActiveCactus = false;
}

void endless() {
	globalReset();
	VGA.clear();
	drawGroundLine();
	printScoreLabel();
	while (vidas > 0) {

		checkPause();

		/* GET INPUT FROM FPGA BUTTONS*/
		enum event_t ev = hasEvent();
		if (ev != event_none) {
			processEvent(ev);
		}

		if(ev == event_none){
			speedMult = 1;
			IsRunning = false;
		}


		int frame = millis() - LastFrameTime;
		if (frame > FrameDelay) {
			LastFrameTime = millis();

			if(ActiveCactus){
				cactus_posX -= (cactus_velocity * speedMult);
				if (cactus_posX <= -5)
				{
					cactus_posX = 145;
					cactus_posY = Screen_height + (cactus_height / 5);
					randomCactus = rand() % 3;
					ActiveCactus = false;
					}
				}
			playerJump();
			drawDots();		
			updateDustCoordinates();

			drawHearts();

			cloud_x = cloud_x - (1 * speedMult);
			cloud2_x = cloud2_x - (1 * speedMult);
			cloud3_x = cloud3_x - (1 * speedMult);
			drawClouds();
			updateCloudsCoordinates();
			
			drawPlayer();
			player_lastKnown_posX = player_posX;
			player_lastKnown_posY = player_posY;

			drawCactus();
			cactus_lastKnown_posX = cactus_posX;
			cactus_lastKnown_posY = cactus_posY;

			detectCollisionsForObject(cactus_posX, cactus_posY, cactus_width, cactus_height, CACTUS_TYPE);
		}
		if (scoreTimeCount >= 11000) {
			scoreTimeCount = 0;
			score = score + (1 * speedMult);
			printScoreOnScreen();
		}
		if (cactusTimeCount >= 200000) {
			cactusTimeCount = 0;
			cactus_velocity++;
			if (cactus_velocity >= 20)
			{
				cactus_velocity = 20;
			}
		}
		if (cactusSpawnRate >= 25000) {
			cactusSpawnRate = 0;
			if(!ActiveCactus){
				cactusAppear();
			}
		}
		scoreTimeCount++;
		cactusTimeCount++;
		cactusSpawnRate++;
	}
	saveScore(score);
	gameState = GAMEOVER_STATE;
}

void saveScore(int score) {
	if (score > bestScoreFirst) {
		bestScoreThird = bestScoreSecond;
		bestScoreSecond = bestScoreFirst;
		bestScoreFirst = score;
	}
	else if (score > bestScoreSecond) {
		bestScoreThird = bestScoreSecond;
		bestScoreSecond = score;
	}
	else if (score > bestScoreThird) {
		bestScoreThird = score;
	}
}

void clearClouds(){
	VGA.setColor(BLACK);
	VGA.clearArea(cloud_lastKnown_x, cloud_lastKnown_y, cloud_width, cloud_height);
	VGA.clearArea(cloud2_lastKnown_x, cloud2_lastKnown_y, cloud2_width, cloud2_height);
	VGA.clearArea(cloud3_lastKnown_x, cloud3_lastKnown_y, cloud3_width, cloud3_height);
}

void gameOver(){
	if (!gameOverRendered) {
		drawHearts();
		clearClouds();
		IsDead = true;
		IsRunning = false;
		drawPlayer();
		VGA.setColor(WHITE);
		VGA.printtext(45, 40, "Game Over");
		VGA.setColor(BLUE);
		VGA.printtext(45, 60, "Press 4 to");
		VGA.printtext(45, 70, "continue!");
		VGA.setColor(WHITE);
		gameOverRendered = true;
	}
	checkInputFromGameOver();
}

void scores() {
	if (!scoreRendered) {
		VGA.clear();
		VGA.setColor(BLUE);
		VGA.printtext(45, 30, "Best Scores!!");
		VGA.setColor(YELLOW);
		char bestScore1Array [4];
		char bestScore2Array [4];
		char bestScore3Array [4];

		itoa(bestScoreFirst, bestScore1Array, 10);
		itoa(bestScoreSecond, bestScore2Array, 10);
		itoa(bestScoreThird, bestScore3Array, 10);

		VGA.setColor(WHITE);

		VGA.printtext(70, 45, bestScore1Array);
		VGA.printtext(70, 60, bestScore2Array);
		VGA.printtext(70, 75, bestScore3Array);

		scoreRendered = true;
	}
	checkInputFromScores();
}

void instructions() {
	if (!instructionsRendered) {
		VGA.clear();
		VGA.setColor(BLUE);
		VGA.printtext(40, 30, "Instructions");
		VGA.setColor(WHITE);
		VGA.printtext(45, 45, "Btn1 Jump");
		VGA.printtext(42, 60, "Btn2 Go down");
		VGA.printtext(42, 75, "Btn3 Run");
		instructionsRendered = true;
	}
	checkInputFromInstructions();
}

void detectCollisionsForObject(int x, int y, int width, int height, int type)
{
	if (player_posX < (x + width)
			&& (player_posX + player_width) > x
			&& player_posY < (y + height)
			&& (player_posY + player_height) > y)
	{
		if (type == CACTUS_TYPE)
		{
			cactus_posX = 145;
			cactus_posY = Screen_height + (cactus_height / 5);
			vidas--;
			randomCactus = rand() % 3;
			ActiveCactus = false;
		}
	}
}

void updateDustCoordinates(){
	dust_lastKnown_x = dust_x;
	dust_lastKnown_y = dust_y;

	dust_lastKnown_x2 = dust_x2;
	dust_lastKnown_y2 = dust_y2;

	dust_lastKnown_x3 = dust_x3;
	dust_lastKnown_y3 = dust_y3;

	dust_lastKnown_x4 = dust_x4;
	dust_lastKnown_y4 = dust_y4;

	dust_lastKnown_x5 = dust_x5;
	dust_lastKnown_y5 = dust_y5;

	dust_lastKnown_x6 = dust_x6;
	dust_lastKnown_y6 = dust_y6;

	dust_lastKnown_x7 = dust_x7;
	dust_lastKnown_y7 = dust_y7;

	dust_lastKnown_x8 = dust_x8;
	dust_lastKnown_y8 = dust_y8;

	if (dust_x <= 0){
		dust_x = 160;
	}
	if (dust_x2 <= 0){
		dust_x2 = 160;
	}
	if (dust_x3 <= 0){
		dust_x3 = 160;
	}
	if (dust_x4 <= 0){
		dust_x4 = 160;
	}
	if (dust_x5 <= 0){
		dust_x5 = 160;
	}
	if (dust_x6 <= 0){
		dust_x6 = 160;
	}
	if (dust_x7 <= 0){
		dust_x7 = 160;
	}
	if (dust_x8 <= 0){
		dust_x8 = 160;
	}
}

void updateCloudsCoordinates(){
	cloud_lastKnown_x = cloud_x;
	cloud_lastKnown_y = cloud_y;

	cloud2_lastKnown_x = cloud2_x;
	cloud2_lastKnown_y = cloud2_y;

	cloud3_lastKnown_x = cloud3_x;
	cloud3_lastKnown_y = cloud3_y;

	if (cloud_x <= 0){
		cloud_x = 145;
		cloud_y = (rand()%(40-10))+10;
	}
	if (cloud2_x <= 0){
		cloud2_x = 145;
		cloud2_y = (rand()%(40-10))+10;
	}
	if (cloud3_x <= 0){
		cloud3_x = 145;
		cloud3_y = (rand()%(40-10))+10;
	}
}