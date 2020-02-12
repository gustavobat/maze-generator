//
// Created by Gustavo on 07/02/2020.
//

#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <vector>

typedef std::vector<std::vector<int>> MazeGrid;

struct MazeGeneratorManager {
    MazeGrid grid;
    int rows = 0;
    int columns = 0;
    std::pair<int, int> currentPosition{-1, -1};
};


void InitializeMazeGrid(MazeGeneratorManager &maze);

void FillExternalWalls(MazeGeneratorManager &maze);

void ChooseInitialCell(MazeGeneratorManager &maze);

void PrintMaze(MazeGeneratorManager &maze);

void PrintMazeMatrix(MazeGeneratorManager &manager);

int RandomInteger(int a, int b);

bool Move(MazeGeneratorManager &maze);

void CleanUpCellValues(MazeGeneratorManager &maze);

#endif //MAZEGENERATOR_H
