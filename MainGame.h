#include "Sound.h"
#include "Input.h"
#include "Screen.h"
#include "VGA.h"
#include <stdlib.h>

unsigned const int FrameDelay = 120;
unsigned long LastFrameTime;

const char MENU_STATE = 0;
const char ENDLESS_STATE = 1;
const char GAMEOVER_STATE = 2;
const char SCORES_STATE = 3;
const char INSTRUCTIONS_STATE = 4;

char gameState = MENU_STATE;
int timerCounter = 0;
long xRandomPos;
int vidas = 3;
int score = 0;

int bestScoreFirst = 0;
int bestScoreSecond = 0;
int bestScoreThird = 0;
int scoreTimeCount = 0;
int cactusTimeCount = 0;

void menu();
void endless();
void gameOver();
void checkInputFromMenu();
void checkCollisions();
void scores();
void saveScore(int score);
void instructions();
void detectCollisions();
void detectCollisionsForObject(int x, int y, int width, int height, int type);

boolean scoreRendered = false;
boolean instructionsRendered = false;
boolean gameOverRendered = false;

#include "CheckInputs.h"

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
		if (player_posY <= 30) { // as higher as he can get
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
}

void menu() {
	if (PlayingSound) {
		AudioFillBuffer();
	}
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

	int heart1x = 10;
	int heart1y = 0;

	int heart2x = 25;
	int heart2y = 0;

	int heart3x = 40;
	int heart3y = 0;

	VGA.setColor(BLACK);

	VGA.clearArea(heart1x, heart1y, heartWidth, heartHeight);
	VGA.clearArea(heart2x, heart2y, heartWidth, heartHeight);
	VGA.clearArea(heart3x, heart3y, heartWidth, heartHeight);

	if (vidas > 0) {
		VGA.writeArea(heart1x, heart1y, heartWidth, heartHeight, heartcontainer);
		if (vidas > 1) {
			VGA.writeArea(heart2x, heart2y, heartWidth, heartHeight, heartcontainer);
			if (vidas > 2) {
				VGA.writeArea(heart3x, heart3y, heartWidth, heartHeight, heartcontainer);
			}
		}
	}
}

void redraw() {

}

void printScoreOnScreen() {
	char sc [4];
	itoa(score, sc, 10);
	VGA.setColor(WHITE);
	VGA.printtext(120, 0, sc);
}

void printScoreLabel() {
	VGA.setColor(BLUE);
	VGA.printtext(70, 0, "Score:");
	VGA.setColor(WHITE);
}

void endless() {
	vidas = 3;
	score = 0;
	cactus_velocity = 6;
	VGA.clear();
	drawGroundLine();
	printScoreLabel();
	while (vidas > 0) {

		/* GET INPUT FROM FPGA BUTTONS*/
		enum event_t ev = hasEvent();
		if (ev != event_none) {
			processEvent(ev);
		}


		int frame = millis() - LastFrameTime;
		if (frame > FrameDelay) {
			LastFrameTime = millis();

			drawPlayer();
			player_lastKnown_posX = player_posX;
			player_lastKnown_posY = player_posY;

			cactus_posX -= cactus_velocity;
			if (cactus_posX <= -15)
			{
				cactus_posX = 145;
				cactus_posY = Screen_height + (cactus_height / 5);
			}
			drawCactus();
			cactus_lastKnown_posX = cactus_posX;
			cactus_lastKnown_posY = cactus_posY;

			playerJump();
			drawDots();
			drawHearts();

			detectCollisions();
		}
		if (scoreTimeCount >= 11000) {
			scoreTimeCount = 0;
			score ++;
			printScoreOnScreen();
		}
		if (cactusTimeCount >= 400000) {
			cactusTimeCount = 0;
			cactus_velocity++;
			if (cactus_velocity >= 11)
			{
				cactus_velocity = 11;
			}
		}
		scoreTimeCount++;
		cactusTimeCount++;
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

void gameOver() {
	if (PlayingSound) {
		AudioFillBuffer();
	}
	if (!gameOverRendered) {
		VGA.clear();
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
	if (PlayingSound) {
		AudioFillBuffer();
	}
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
	if (PlayingSound) {
		AudioFillBuffer();
	}
	if (!instructionsRendered) {
		VGA.clear();
		VGA.setColor(BLUE);
		VGA.printtext(40, 30, "Instructions");
		VGA.setColor(WHITE);
		VGA.printtext(45, 45, "Btn1 Jump");
		VGA.printtext(42, 60, "Btn2 go Down");
		instructionsRendered = true;
	}
	checkInputFromInstructions();
}

void detectCollisions() {
	/*CACTUS*/
	detectCollisionsForObject(cactus_posX, cactus_posY, cactus_width, cactus_height, CACTUS_TYPE);
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
		}
	}
}
