//
// Created by Gustavo on 07/02/2020.
//

#include <iostream>
#include <vector>
#include <random>

typedef std::vector<std::vector<int>> MazeGrid;


struct MazeGeneratorManager {
    MazeGrid grid;
    int rows = 0;
    int columns = 0;
};

MazeGrid InitializeMazeGrid(MazeGeneratorManager& manager);

void PrintMaze(MazeGeneratorManager& manager);

int main() {
    
    MazeGeneratorManager manager;
    manager.rows = 10;
    manager.columns = 20;
    
    MazeGrid grid = InitializeMazeGrid(manager);
    
    int nCellsAtBorder = 2 * (manager.rows + manager.columns) - 2;
    std::vector<int> borderCells(nCellsAtBorder);
    for (int j = 0 ; j < manager.columns; j++) {
        borderCells[j] = j;
        borderCells[j + manager.columns] = j + manager.columns  * (manager.rows - 1);
    }
    
    for (int i = 1 ; i < manager.rows - 1; i++) {
        borderCells[2 * manager.columns + 2 * i] = manager.columns * (i + 1) + 0;
        borderCells[2 * manager.columns + 2 * i + 1] = manager.columns * (i + 2) - 1;
    }
    
    for (int i = 0 ; i < borderCells.size(); i++) {
        std::cout << borderCells[i] << '\n';
    }
    
    PrintMaze(manager);
    
    return 0;
}

MazeGrid InitializeMazeGrid(MazeGeneratorManager& manager) {
    
    if (manager.rows % 2 == 0) {
        manager.rows += 1;
        std::cout << "The number of rows must be odd, switching it to: " << manager.rows << ".\n";
    }
    if (manager.columns % 2 == 0) {
        manager.columns += 1;
        std::cout << "The number of columns must be odd, switching it to: " << manager.columns << ".\n";
    }
    
    manager.grid.resize(manager.rows);
    for (int i =0; i < manager.rows; i++) {
        manager.grid[i].resize(manager.columns);
        for (int j =0; j < manager.columns; j++) {
            manager.grid[i][j] = -1;
        }
    }
    
    return manager.grid;
}

void PrintMaze(MazeGeneratorManager& manager) {
    std::cout << ":: Printing Grid State:\n";
    for (int i = 0; i < manager.grid.size(); i++) {
        std::cout << "  ";
        for (int j =0; j < manager.grid[i].size(); j++) {
            std::cout << manager.grid[i][j];
            j == manager.grid[i].size() - 1 ? std::cout << '\n' : std::cout << ' ';
        }
    }
}
