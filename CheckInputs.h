void checkInputFromMenu() {
	if (!digitalRead(FPGA_SW_0)) {
		if (digitalRead(FPGA_BTN_0)) {
			gameState = ENDLESS_STATE;
			VGA.clear();
		}
		if (digitalRead(FPGA_BTN_1)) {
			gameState = SCORES_STATE;
			VGA.clear();
		}
		if (digitalRead(FPGA_BTN_2)) {
			gameState = INSTRUCTIONS_STATE;
			VGA.clear();
		}
	}
}

void checkInputFromEndless() {
	if (!digitalRead(FPGA_SW_0)) {
		if (digitalRead(FPGA_BTN_0)) {

		}
		if (digitalRead(FPGA_BTN_1)) {

		}
		if (digitalRead(FPGA_BTN_3)) {
			gameState = MENU_STATE;
			VGA.clear();
		}
	}
}

void checkInputFromGameOver() {
	if (!digitalRead(FPGA_SW_0)) {
		if (digitalRead(FPGA_BTN_3)) {
			gameState = MENU_STATE;
			gameOverRendered = false;
			VGA.clear();
		}
	}
}

void checkInputFromScores() {
	if (!digitalRead(FPGA_SW_0)) {
		if (digitalRead(FPGA_BTN_3)) {
			gameState = MENU_STATE;
			scoreRendered = false;
			VGA.clear();
		}
	}
}

void checkInputFromInstructions() {
	if (!digitalRead(FPGA_SW_0)) {
		if (digitalRead(FPGA_BTN_3)) {
			gameState = MENU_STATE;
			instructionsRendered = false;
			VGA.clear();
		}
	}
}