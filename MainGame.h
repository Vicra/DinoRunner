#include "Sound.h"
#include "Screen.h"
#include "VGA.h"
#include <stdlib.h>


unsigned const int FrameDelay = 100;
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

void menu();
void endless();
void gameOver();
void checkInputFromMenu();
void checkCollisions();
void scores();
void saveScore(int score);
void instructions();

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

void menu() {
	if (PlayingSound) {
		AudioFillBuffer();
	}
	VGA.printtext(50, 25, "Dino");
	VGA.printtext(50, 40, "Runner!");
	if (!digitalRead(FPGA_SW_0)) {
		VGA.setColor(BLUE);
	}
	VGA.printtext(40, 53, "1.Start Game");
	VGA.printtext(40, 66, "2.Scores");
	VGA.printtext(40, 79, "3.Instructions");
	VGA.setColor(WHITE);
	if (digitalRead(FPGA_SW_0)) {
		VGA.setColor(BLUE);
	}
	VGA.setColor(WHITE);
	checkInputFromMenu();
}

void printScoreOnScreen() {
	char sc [4];
	itoa(score, sc, 10);
	VGA.setColor(WHITE);
	VGA.printtext(110, 5, "Sc");
	VGA.printtext(135, 5, sc);
	VGA.setColor(WHITE);
}

void endless() {
	if((millis() - LastFrameTime) > FrameDelay){
		LastFrameTime = millis();
		drawPlayer();
		player_lastKnown_posX = player_posX;
      	player_lastKnown_posY = player_posY;
	}
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
		VGA.printtext(45, 40, "Game Over");
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
		VGA.printtext(45, 30, "Best Scores");

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
		VGA.printtext(45, 30, "Instructions");
		VGA.setColor(WHITE);
		VGA.printtext(45, 45, "Btn1 Jump");
		VGA.printtext(45, 60, "Btn2 Duck");
		instructionsRendered = true;
	}
	checkInputFromInstructions();
}
