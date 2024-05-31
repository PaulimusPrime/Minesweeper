#include "Tile.h"



// For aa sette Tilelabel i henhold til state
const std::map<Cell, std::string> cellToSymbol{{Cell::closed, ""},
									 			{Cell::open, ""},
									 	  		{Cell::flagged, "|>"}};

Tile::Tile(TDT4102::Point pos, int size) : 
	Button({pos.x, pos.y}, 1.5*size, size, "") {
		setButtonColor(TDT4102::Color::silver);
	}

void Tile::open()
{

	if ((state == Cell::closed) && getMine()) {
		setButtonColor(TDT4102::Color::black);
		set_label_color(TDT4102::Color::red);
		set_label("X"); 
		state = Cell::open;
	}
	else if (state == Cell::closed) {
		setButtonColor(TDT4102::Color::white);
		state = Cell::open;
		

		if (isMine) {
			set_label("X");
			set_label_color(TDT4102::Color::red);
		}
	}
}

void Tile::flag()
{
	if (getState() == Cell::flagged) {
		
		set_label(cellToSymbol.at(Cell::closed));
		state = Cell::closed;
		
	}
	else if (getState() == Cell::closed) {
		state = Cell::flagged;
		set_label(cellToSymbol.at(Cell::flagged));
		set_label_color(TDT4102::Color::black);
	}
}

void Tile::closeTile() {
	setButtonColor(TDT4102::Color::silver);
	state = Cell::closed;
	set_label("");
	set_label_color(TDT4102::Color::silver);
}

void Tile::setAdjMines(int n) {
	if (n > 0) {
		set_label(std::to_string(n));
		set_label_color(minesToColor.at(n));
	}
}
