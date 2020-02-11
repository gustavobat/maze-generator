//
// Created by Gustavo on 07/02/2020.
//

#include <iostream>
#include <vector>
#include <random>
#include <unistd.h>

typedef std::vector<std::vector<int>> MazeGrid;

struct MazeGeneratorManager {
    MazeGrid grid;
    int rows = 0;
    int columns = 0;
    std::pair<int, int> currentPosition{-1, -1};
};

void InitializeMazeGrid(MazeGeneratorManager &manager);

void PrintMaze(MazeGeneratorManager &manager);

std::pair<int, int> ChooseInitialCell(MazeGeneratorManager &maze);

int RandomInteger(int a, int b);

void Move(MazeGeneratorManager &maze);

int main() {

    MazeGeneratorManager manager;
    manager.rows = 10;
    manager.columns = 20;

    InitializeMazeGrid(manager);

    int nCellsAtBorder = 2 * (manager.rows + manager.columns) - 2;
    std::vector<int> borderCells(nCellsAtBorder);
    for (int j = 0; j < manager.columns; j++) {
        manager.grid[0][j] = 0;
        manager.grid[manager.rows - 1][manager.columns - 1 - j] = 0;
#ifdef VERBOSE
        PrintMaze(manager);
#endif
    }

    for (int i = 1; i < manager.rows - 1; i++) {

        manager.grid[i][0] = 0;
        manager.grid[manager.rows - 1 - i][manager.columns - 1] = 0;
#ifdef VERBOSE
        PrintMaze(manager);
#endif
    }

    manager.currentPosition = ChooseInitialCell(manager);

    while (1) {
        Move(manager);
    }

    return 0;
}

void InitializeMazeGrid(MazeGeneratorManager &manager) {

    if (manager.rows % 2 == 0) {
        manager.rows += 1;
        std::cout << "The number of rows must be odd, switching it to: " << manager.rows << ".\n";
    }
    if (manager.columns % 2 == 0) {
        manager.columns += 1;
        std::cout << "The number of columns must be odd, switching it to: " << manager.columns << ".\n";
    }

    manager.grid.resize(manager.rows);
    for (int i = 0; i < manager.rows; i++) {
        manager.grid[i].resize(manager.columns);
        for (int j = 0; j < manager.columns; j++) {
            manager.grid[i][j] = -1;
        }
    }
}

void PrintMaze(MazeGeneratorManager &manager) {

    usleep(200000);
    std::cout << ":: Printing Grid State:\n";
    std::cout.flush();
    system("clear");
    for (int i = 0; i < manager.grid.size(); i++) {
        std::cout << "  ";
        for (int j = 0; j < manager.grid[i].size(); j++) {
            if (i == manager.currentPosition.first && j == manager.currentPosition.second) {
                std::cout << "\033[33m██\033[0m";
            } else {
                if (manager.grid[i][j] == -1) std::cout << "\033[31m██\033[0m";
                if (manager.grid[i][j] == 0) std::cout << "\033[30m██\033[0m";
                if (manager.grid[i][j] == 1) std::cout << "\033[37m██\033[0m";
                if (manager.grid[i][j] == 2) std::cout << "\033[34m██\033[0m";
            }

            if (j == manager.grid[i].size() - 1) std::cout << '\n';
        }
    }
}

std::pair<int, int> ChooseInitialCell(MazeGeneratorManager &maze) {

    int i = RandomInteger(1, maze.rows - 2);
    int j = RandomInteger(1, maze.columns - 2);

    if (i % 2 == 0) i--;
    if (j % 2 == 0) j--;

    maze.grid[i][j] = 1;
    PrintMaze(maze);
    std::pair<int, int> a;

    a.first = i;
    a.second = j;

    return a;
}

int RandomInteger(int a, int b) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(a, b);

    return dis(gen);
}

void Move(MazeGeneratorManager &maze) {
    // Verifies possible movements
    // 0: Move UP
    // 1: Move DOWN
    // 2: Move RIGHT
    // 3: Move LEFT
    std::vector<int> allowedDirections;

    int current_i = maze.currentPosition.first;
    int current_j = maze.currentPosition.second;

    int rows = maze.rows;
    int columns = maze.columns;

    // Checks if cell above is inside the grid, is not a wall and has not yet been visited
    if ((current_i - 2 >= 0) && (maze.grid[current_i - 1][current_j] == -1)) {
        allowedDirections.push_back(0);
    }

    // Checks if cell bellow is inside the grid, is not a wall and has not yet been visited
    if ((current_i + 2 < maze.rows) && (maze.grid[current_i + 1][current_j] == -1)) {
        allowedDirections.push_back(1);
    }

    // Checks if right cell is inside the grid, is not a wall and has not yet been visited
    if ((current_j+ 2 < maze.columns) && (maze.grid[current_i][current_j + 1] == -1)) {
        allowedDirections.push_back(2);
    }

    // Checks if left cell is inside the grid, is not a wall and has not yet been visited
    if ((current_j - 2 >= 0) && (maze.grid[current_i][current_j - 1] == -1)) {
        allowedDirections.push_back(3);
    }

    // At this point we need to go back to previous position and mark the current cell as revisited
    if (allowedDirections.empty()) {

        if (maze.grid[current_i - 1][current_j] == 1) {
            allowedDirections.push_back(0);
        }

        if (maze.grid[current_i + 1][current_j] == 1) {
            allowedDirections.push_back(1);
        }

        if (maze.grid[current_i][current_j + 1] == 1) {
            allowedDirections.push_back(2);
        }

        if (maze.grid[current_i][current_j - 1] == 1) {
            allowedDirections.push_back(3);
        }


        if (allowedDirections.empty()) {
            exit(4);
        }
    }

    int direction = RandomInteger(0, allowedDirections.size() - 1);

    switch (allowedDirections[direction]) {
        // 0: Move UP
        case 0:
            if (maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second] == -1) {
                maze.grid[maze.currentPosition.first][maze.currentPosition.second] = 1;
                maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second] = 1;

                if (maze.grid[maze.currentPosition.first][maze.currentPosition.second - 1] == -1)
                    maze.grid[maze.currentPosition.first][maze.currentPosition.second - 1] = 0;

                if (maze.grid[maze.currentPosition.first][maze.currentPosition.second + 1] == -1)
                    maze.grid[maze.currentPosition.first][maze.currentPosition.second + 1] = 0;

                if (maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second - 1] == -1)
                    maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second - 1] = 0;

                if (maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second + 1] == -1)
                    maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second + 1] = 0;
            }
            // If going backwards
            else if (maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second] == 1) {
                maze.grid[maze.currentPosition.first][maze.currentPosition.second] = 2;
                maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second] = 2;
            }

            maze.currentPosition.first -= 2;
            break;

            // 1: Move DOWN
        case 1:
            if (maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second] == -1) {
                maze.grid[maze.currentPosition.first][maze.currentPosition.second] = 1;
                maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second] = 1;

                if (maze.grid[maze.currentPosition.first][maze.currentPosition.second - 1] == -1)
                    maze.grid[maze.currentPosition.first][maze.currentPosition.second - 1] = 0;

                if (maze.grid[maze.currentPosition.first][maze.currentPosition.second + 1] == -1)
                    maze.grid[maze.currentPosition.first][maze.currentPosition.second + 1] = 0;

                if (maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second - 1] == -1)
                    maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second - 1] = 0;

                if (maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second + 1] == -1)
                    maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second + 1] = 0;
            }

            // If going backwards
            else if (maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second] == 1) {
                maze.grid[maze.currentPosition.first][maze.currentPosition.second] = 2;
                maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second] = 2;
            }

            maze.currentPosition.first += 2;
            break;

        // 2: Move RIGHT
        case 2:
            if (maze.grid[maze.currentPosition.first][maze.currentPosition.second + 1] == -1) {
                maze.grid[maze.currentPosition.first][maze.currentPosition.second] = 1;
                maze.grid[maze.currentPosition.first][maze.currentPosition.second + 1] = 1;

                if (maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second] == -1)
                    maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second] = 0;

                if (maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second] == -1)
                    maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second] = 0;

                if (maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second + 1] == -1)
                    maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second + 1] = 0;

                if (maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second + 1] == -1)
                    maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second + 1] = 0;
            }

            // If going backwards
            else if (maze.grid[maze.currentPosition.first][maze.currentPosition.second + 1] == 1) {
                maze.grid[maze.currentPosition.first][maze.currentPosition.second] = 2;
                maze.grid[maze.currentPosition.first][maze.currentPosition.second + 1] = 2;
            }

            maze.currentPosition.second += 2;
            break;

        // 3: Move LEFT
        case 3:
            if (maze.grid[maze.currentPosition.first][maze.currentPosition.second - 1] == 1) {
                maze.grid[maze.currentPosition.first][maze.currentPosition.second] = 1;
                maze.grid[maze.currentPosition.first][maze.currentPosition.second - 1] = 1;

                if (maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second] == -1)
                    maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second] = 0;

                if (maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second] == -1)
                    maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second] = 0;

                if (maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second - 1] == -1)
                    maze.grid[maze.currentPosition.first - 1][maze.currentPosition.second - 1] = 0;

                if (maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second - 1] == -1)
                    maze.grid[maze.currentPosition.first + 1][maze.currentPosition.second - 1] = 0;
            }

            // If going backwards
            else if (maze.grid[maze.currentPosition.first][maze.currentPosition.second - 1] == 1) {
                maze.grid[maze.currentPosition.first][maze.currentPosition.second] = 2;
                maze.grid[maze.currentPosition.first][maze.currentPosition.second - 1] = 2;
            }

            maze.currentPosition.second -= 2;
            break;

        default:
            exit(0);
    }
    std::cout << allowedDirections[direction] << std::endl;

    PrintMaze(maze);
    // Checks if right cell is inside the grid
    //if (current_i + 2 > columns + 1)


}