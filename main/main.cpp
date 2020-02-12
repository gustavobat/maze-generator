//
// Created by Gustavo on 07/02/2020.
//

#include "MazeGenerator.h"

int main() {

    MazeGeneratorManager maze;
    maze.rows = 31;
    maze.columns = 31;

    InitializeMazeGrid(maze);
    FillExternalWalls(maze);
    ChooseInitialCell(maze);

    while (Move(maze)) {}

    CleanUpCellValues(maze);
    PrintMaze(maze);
    PrintMazeMatrix(maze);

    return 0;
}
