//
// Created by Gustavo on 07/02/2020.
//

#include <iostream>
#include <random>
#include "MazeGenerator.h"

void InitializeMazeGrid(MazeGeneratorManager &maze) {

    if (maze.rows % 2 == 0) {
        maze.rows += 1;
        std::cout << "The number of rows must be odd, switching it to: " << maze.rows << ".\n";
    }
    if (maze.columns % 2 == 0) {
        maze.columns += 1;
        std::cout << "The number of columns must be odd, switching it to: " << maze.columns << ".\n";
    }

    maze.grid.resize(maze.rows);
    for (int i = 0; i < maze.rows; i++) {
        maze.grid[i].resize(maze.columns);
        for (int j = 0; j < maze.columns; j++) {
            maze.grid[i][j] = -1;
        }
    }
}

void FillExternalWalls(MazeGeneratorManager & maze) {
    int nCellsAtBorder = 2 * (maze.rows + maze.columns) - 2;
    std::vector<int> borderCells(nCellsAtBorder);
    for (int j = 0; j < maze.columns; j++) {
        maze.grid[0][j] = 0;
        maze.grid[maze.rows - 1][maze.columns - 1 - j] = 0;
    }

    for (int i = 1; i < maze.rows - 1; i++) {

        maze.grid[i][0] = 0;
        maze.grid[maze.rows - 1 - i][maze.columns - 1] = 0;
    }
}

void ChooseInitialCell(MazeGeneratorManager &maze) {

    int i = RandomInteger(1, maze.rows - 2);
    int j = RandomInteger(1, maze.columns - 2);

    if (i % 2 == 0) i--;
    if (j % 2 == 0) j--;

    maze.grid[i][j] = 1;

    maze.currentPosition = std::make_pair(i, j);

#ifdef VERBOSE
    PrintMaze(maze);
#endif

}

void PrintMaze(MazeGeneratorManager &maze) {

    std::cout << ":: Printing Grid State:\n\n";
    std::cout.flush();
    system("clear");
    for (int i = 0; i < maze.grid.size(); i++) {
        std::cout << "  ";
        for (int j = 0; j < maze.grid[i].size(); j++) {
            if (i == maze.currentPosition.first && j == maze.currentPosition.second) {
                std::cout << "\033[33m██\033[0m";
            } else {
                if (maze.grid[i][j] == -1) std::cout << "\033[30m██\033[0m";
                if (maze.grid[i][j] == 0) std::cout << "\033[30m██\033[0m";
                if (maze.grid[i][j] == 1) std::cout << "\033[37m██\033[0m";
                if (maze.grid[i][j] == 2) std::cout << "\033[34m██\033[0m";
            }

            if (j == maze.grid[i].size() - 1) std::cout << '\n';
        }
    }
    std::cout << '\n';
}

void PrintMazeMatrix(MazeGeneratorManager &manager) {

    std::cout << ":: Printing Grid:\n\n";
    std::cout.flush();
    for (int i = 0; i < manager.rows; i++) {
        std::cout << "  ";
        for (int j = 0; j < manager.columns; j++) {
            std::cout << manager.grid[i][j];
            j == manager.grid[i].size() - 1 ? std::cout << '\n' : std::cout << ' ';
        }
    }
    std::cout << '\n';
}

int RandomInteger(int a, int b) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(a, b);

    return dis(gen);
}

bool Move(MazeGeneratorManager &maze) {

#ifdef VERBOSE
    PrintMaze(maze);
#endif

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

    // Checks if candidate cell is inside the grid and has not been visited yet
    if ((current_i - 2 >= 0) && (maze.grid[current_i - 2][current_j] == -1)) {
        allowedDirections.push_back(0);
    }
    if ((current_i + 2 < maze.rows) && (maze.grid[current_i + 2][current_j] == -1)) {
        allowedDirections.push_back(1);
    }

    if ((current_j+ 2 < maze.columns) && (maze.grid[current_i][current_j + 2] == -1)) {
        allowedDirections.push_back(2);
    }
    if ((current_j - 2 >= 0) && (maze.grid[current_i][current_j - 2] == -1)) {
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

        // End of program
        if (allowedDirections.empty()) {
            maze.currentPosition.first = -1;
            maze.currentPosition.second = -1;
            return false;
        }
    }

    int direction = RandomInteger(0, static_cast<int>(allowedDirections.size() - 1));

    switch (allowedDirections[direction]) {
        // 0: Move UP
        case 0:
            if (maze.grid[current_i - 2][current_j] == -1) {
                maze.grid[current_i][current_j] = 1;
                maze.grid[current_i - 1][current_j] = 1;

                if (maze.grid[current_i][current_j - 1] == -1)
                    maze.grid[current_i][current_j - 1] = 0;

                if (maze.grid[current_i][current_j + 1] == -1)
                    maze.grid[current_i][current_j + 1] = 0;

                //if (maze.grid[current_i + 1][current_j] == -1)
                //    maze.grid[current_i + 1][current_j] = 0;

                if (maze.grid[current_i - 1][current_j - 1] == -1)
                    maze.grid[current_i - 1][current_j - 1] = 0;

                if (maze.grid[current_i - 1][current_j + 1] == -1)
                    maze.grid[current_i - 1][current_j + 1] = 0;
            }
                // If going backwards
            else if (maze.grid[current_i - 2][current_j] == 1) {
                maze.grid[current_i][current_j] = 2;
                maze.grid[current_i - 1][current_j] = 2;
            }

            maze.currentPosition.first -= 2;
            break;

            // 1: Move DOWN
        case 1:
            if (maze.grid[current_i + 2][current_j] == -1) {
                maze.grid[current_i][current_j] = 1;
                maze.grid[current_i + 1][current_j] = 1;

                if (maze.grid[current_i][current_j - 1] == -1)
                    maze.grid[current_i][current_j - 1] = 0;

                if (maze.grid[current_i][current_j + 1] == -1)
                    maze.grid[current_i][current_j + 1] = 0;

                if (maze.grid[current_i + 1][current_j - 1] == -1)
                    maze.grid[current_i + 1][current_j - 1] = 0;

                if (maze.grid[current_i + 1][current_j + 1] == -1)
                    maze.grid[current_i + 1][current_j + 1] = 0;
            }

                // If going backwards
            else if (maze.grid[current_i + 2][current_j] == 1) {
                maze.grid[current_i][current_j] = 2;
                maze.grid[current_i + 1][current_j] = 2;
            }

            maze.currentPosition.first += 2;
            break;

            // 2: Move RIGHT
        case 2:
            if (maze.grid[current_i][current_j + 2] == -1) {
                maze.grid[current_i][current_j] = 1;
                maze.grid[current_i][current_j + 1] = 1;

                if (maze.grid[current_i - 1][current_j] == -1)
                    maze.grid[current_i - 1][current_j] = 0;

                if (maze.grid[current_i + 1][current_j] == -1)
                    maze.grid[current_i + 1][current_j] = 0;

                if (maze.grid[current_i - 1][current_j + 1] == -1)
                    maze.grid[current_i - 1][current_j + 1] = 0;

                if (maze.grid[current_i + 1][current_j + 1] == -1)
                    maze.grid[current_i + 1][current_j + 1] = 0;
            }

                // If going backwards
            else if (maze.grid[current_i][current_j + 2] == 1) {
                maze.grid[current_i][current_j] = 2;
                maze.grid[current_i][current_j + 1] = 2;
            }

            maze.currentPosition.second += 2;
            break;

            // 3: Move LEFT
        case 3:
            if (maze.grid[current_i][current_j - 2] == -1) {
                maze.grid[current_i][current_j] = 1;
                maze.grid[current_i][current_j - 1] = 1;

                if (maze.grid[current_i - 1][current_j] == -1)
                    maze.grid[current_i - 1][current_j] = 0;

                if (maze.grid[current_i + 1][current_j] == -1)
                    maze.grid[current_i + 1][current_j] = 0;

                if (maze.grid[current_i - 1][current_j - 1] == -1)
                    maze.grid[current_i - 1][current_j - 1] = 0;

                if (maze.grid[current_i + 1][current_j - 1] == -1)
                    maze.grid[current_i + 1][current_j - 1] = 0;
            }

                // If going backwards
            else if (maze.grid[current_i][current_j - 2] == 1) {
                maze.grid[current_i][current_j] = 2;
                maze.grid[current_i][current_j - 1] = 2;
            }

            maze.currentPosition.second -= 2;
            break;

        default:
            exit(-1);
    }
    return true;
}

void CleanUpCellValues(MazeGeneratorManager &maze) {

    for (int i = 0; i < maze.rows; i++) {
        for (int j = 0; j < maze.rows; j++) {
            // Fix some wall cells that were not marked during the process,
            // since it could block a path that, at the time, was valid.
            if (maze.grid[i][j] == -1) {
                maze.grid[i][j] = 0;
            }
            // At the end of the process all path cells are visited twice and
            // marked as '2'. This conditions ensures the valus of the
            // resulting cells is either '0' or '1'.
            if (maze.grid[i][j] == 2) {
                maze.grid[i][j] = 1;
            }
        }
    }
}
