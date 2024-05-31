#include "MinesweeperWindow.h"
#include <iostream>
/// @brief Constructor of game window
/// @param x : x value of top left window corner
/// @param y : y value of top left window corner
/// @param width : width of tiles
/// @param height : height of tiles
/// @param mines : Amount of mines on the board
/// @param title : Title of game window
MinesweeperWindow::MinesweeperWindow(int x, int y, int width, int height, int mines, const string &title) : 
	// Initialiser medlemsvariabler, bruker konstruktoren til AnimationWindow-klassen
	AnimationWindow{x, y, width * cellSize + 10, (height + 1) * cellSize + 100, title},
	width{width}, height{height}, mines{mines},
	//titleText({100, 0}, 160, 45, "MINESWEEPER"),
	textPopup({5, 400}, 120, 50, "Good Luck"),
	mainButton({5, 450}, 100, 50, "Restart"),
	quitButton({110, 450}, 100, 50, "Quit"),
	remainingBombs({250, 450}, 170, 50, "Mines: " + minesToString())
{
	// Legg til alle tiles i vinduet
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			tiles.emplace_back(new Tile{ Point{j * cellSize, i * cellSize}, cellSize});
			tiles.back()->setCallback(std::bind(&MinesweeperWindow::cb_click, this));
			auto temp = tiles.back().get();
			add(*temp); 
		}
	}
	srand(time(NULL));
	for (int i = 0; i < mines; i++) {
		int tilePos = rand() % tiles.size();
		bool checkIfMine = tiles.at(tilePos)->getMine();
		if (!checkIfMine) {
			tiles.at(tilePos)->setMine(true);
		}
	}


	//TEXTBOXES
	add(textPopup);
	add(remainingBombs);
	
	//FUNCTIONAL BUTTONS
	add(mainButton);
	mainButton.setButtonColor(Color::white);
	mainButton.setCallback(std::bind(&MinesweeperWindow::restart,this));
	add(quitButton);
	quitButton.setButtonColor(Color::white);
	quitButton.setCallback(std::bind(&MinesweeperWindow::quitfunc, this));
}

/// @brief Function to check neighbouring tiles of clicked one
/// @param xy : The point of the mouseclick
/// @return return vector of neighbouring tiles
vector<Point> MinesweeperWindow::adjacentPoints(Point xy) const {
	vector<Point> points;
	for (int di = -1; di <= 1; ++di) {
		for (int dj = -1; dj <= 1; ++dj) {
			if (di == 0 && dj == 0) {
				continue;
			}

			Point neighbour{ xy.x + di * cellSize,xy.y + dj * cellSize };
			if (inRange(neighbour)) {
				points.push_back(neighbour);
			}
		}
	}
	return points;
}
/// @brief Function to open a tile
/// @param xy : The point of the mouseclick
void MinesweeperWindow::openTile(Point xy) {

	//Tile opening condition
	if (MinesweeperWindow::at(xy) -> getState() != Cell::closed) {
		return;
	}
	//Open tile
	MinesweeperWindow::at(xy) -> open();

	//Loss condition
	if (MinesweeperWindow::at(xy) -> getMine()) {
		gameLoss();
	}
	
	std::vector<Point> adjPoints = MinesweeperWindow::adjacentPoints(xy);
	int nearbyMines = countMines(adjPoints);

	//Player info conditions
	if (nearbyMines > 0) {
		MinesweeperWindow::at(xy) -> setAdjMines(nearbyMines);
	}
	else if (nearbyMines == 0) {
		for (const auto& p : adjPoints) {
			MinesweeperWindow::openTile(p);
		}
	}
}
//___________________________________________________

/// @brief Function to flag a tile
/// @param xy 
void MinesweeperWindow::flagTile(Point xy) {
	shared_ptr<Tile>& TilePosition = at(xy);
	if (TilePosition->getState() == Cell::open) {
		return;
	}
	else {
		TilePosition->flag();
		remainingBombs.setText("Mines: " + minesToString());
	}
}

//Kaller openTile ved venstreklikk og flagTile ved hoyreklikk

/// @brief Function to open or flag tiles depending on mouseclick
void MinesweeperWindow::cb_click() {
	
	Point xy{this->get_mouse_coordinates()};

	if (!inRange(xy)) {
		return;
	}
	if (this->is_left_mouse_button_down()) {
		openTile(xy);
	}
	else if(this->is_right_mouse_button_down()){
		flagTile(xy);
	}
}

/// @brief Function to count adjacent mines to opened tile
/// @param coords : Vector containing adjacent tiles
/// @return Amount of adjacent mines
int MinesweeperWindow::countMines(vector<Point> coords) const {
	int minesAdjecent = 0;
	for (const auto& c : coords) {
		if (MinesweeperWindow::at(c)->getMine()) {
			minesAdjecent += 1;
		}
	}
	return minesAdjecent;
}

/// @brief Function to return unopened tiles
/// @param tiles : All tiles in minesweeper window
/// @return Unopened tiles
int MinesweeperWindow::tiles_left(vector<shared_ptr<Tile>> tiles) {
	int remainingTile; 
	int totalTiles;
	for (const auto& t : tiles) {
		if ( t ->getState() == Cell::closed) {
			remainingTile ++;
		}
	}
	return remainingTile;
}

/// @brief Function to check winning condition
void MinesweeperWindow::gameVictory() {
	if (tiles_left(tiles) == mines) {
		for (const auto& t : tiles) {
			t -> open();
		}
		textPopup.setText("Congratulations!");
	}
}

/// @brief Function to check losing condition
void MinesweeperWindow::gameLoss() {
		for (const auto& t : tiles) {
			t -> open();
		}
		textPopup.setText("You lost!");
		remainingBombs.setText("Kaboom");

}

/// @brief Button function to restart game
void MinesweeperWindow::restart() {
	for (const auto& t : tiles) {
		t -> closeTile();
		t -> setMine(false);
	}
	for (int i = 0; i < mines; i++) {
		int tilePos = rand() % tiles.size();
		bool checkIfMine = tiles.at(tilePos)->getMine();
		if (!checkIfMine) {
			tiles.at(tilePos)->setMine(true);
		}
	}
	textPopup.setText("Good luck!");
	remainingBombs.setText("Mines: " + mines);
}

void MinesweeperWindow::quitfunc() {
	MinesweeperWindow::close();
}

std::string MinesweeperWindow::minesToString() {
	int remainingMines = mines;
	for (const auto& t : tiles) {
		if (t -> getState() == Cell::flagged) {
			remainingMines --;
		}
	}
	std::string toString = to_string(remainingMines);
	return toString;
}
