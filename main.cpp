#include "MinesweeperWindow.h"

//------------------------------------------------------------------------------'

// C++ programs start by executing the function main
int main() {
    // Show a nice message in the Terminal window
    constexpr int width = 12;
    constexpr int height = 12;
    constexpr int mines = 12;

    Point leftCornerPos{425, 225};
    MinesweeperWindow mw {leftCornerPos.x, leftCornerPos.y, width, height, mines, "Minesweeper"};
    mw.wait_for_close();
    return 0;
}

//------------------------------------------------------------------------------
