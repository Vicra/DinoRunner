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
int cactusSpawnRate = 0;

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
void updateDustCoordinates();
void updateCloudsCoordinates();

boolean scoreRendered = false;
boolean instructionsRendered = false;
boolean gameOverRendered = false;