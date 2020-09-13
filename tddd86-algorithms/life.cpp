/*
 * Martin Gustafsson @margu424 && Axel Gard @axega544
 *
 * The task was to crate a replication of the game of life.
 *
 * Soruces :
 * labb PM - https://www.ida.liu.se/~TDDD86/info/misc/labb1.pdf
 * C++ doc - http://www.cplusplus.com/doc/tutorial/files/
 * StackOverFlow - https://stackoverflow.com/questions/25225948/how-to-check-if-a-file-exists-in-c-with-fstreamopen?rq=1
 *
 */
#include <iostream>
#include "grid.h"
#include "lifeutil.h"
#include <string>
#include <fstream>

/* Prints the given grid to the terminal */
void printGrid(const Grid<int>& grid){
    for (int i = 0; i < grid.numRows(); i++){
        for(int j = 0; j < grid.numCols(); j++){
            if (grid.get(i,j)){
                std::cout << "X";
            }
            else{
                std::cout << "-";
            }
        }
        std::cout << std::endl;
    }

}

/* returns the number of neighbours in a given grid at pos row and col */
int getNeighbours(const Grid<int>& grid, int row, int col){
    int neighbours = 0;
    for (int i = -1; i < 2; i++){
        for (int j = -1; j < 2; j++){
            if (i == 0 && j == 0) continue;
            if (!grid.inBounds(row+i, col+j)) continue;
            if (grid.get(row+i, col+j)) neighbours++;

        }
    }
    return neighbours;
}

/* Runs the game one genaration forward */
void tick(Grid<int>& grid){
    Grid<int> newGrid = Grid<int>(grid.numRows(), grid.numCols());
    for (int i = 0; i < grid.numRows(); i++){
        for (int j = 0; j < grid.numCols(); j++){
            if (getNeighbours(grid, i,j) == 2) newGrid.set(i,j,grid.get(i,j));
            else if (getNeighbours(grid, i,j) == 3) newGrid.set(i,j,1);
            else newGrid.set(i,j, 0);
        }
    }
    grid = newGrid;
}

/* will run the game forever */
void animate(Grid<int>& grid){
    while (true) {
        clearConsole();
        printGrid(grid);
        pause(100);
        tick(grid);
    }

}


/* main function that handels user input  */
int main() {
    std::cout << "Welcome to the TDDD86 Game of Life,\n"
                 "a simulation of the lifecycle of a bacteria colony.\n"
                 "Cells (X) live and die by the following rules:\n"
                 " - A cell with 1 or fewer neighbours dies.\n"
                 " - Locations with 2 neighbours remain stable.\n"
                 " - Locations with 3 neighbours will create life. \n"
                 " - A cell with 4 or more neighbours dies. \n\n";

    std::string filename;
    std::cout << "Grid input file name? : ";
    std::cin >> filename;
    std::ifstream infile(filename);
    if (!infile.is_open()){
        std::cout << "file not found : " << std::endl;
        return 1;
    }

    int rows;
    int coloums;

    infile >> rows;
    infile >> coloums;

    Grid<int> grid = Grid<int>(rows, coloums);

    // get given grid
    for (int i = 0; i < rows; i++){
        std::string row;
        infile >> row;
        for (int j = 0; j < coloums; j++){
            if (row[j] == 'X'){
                grid.set(i, j, 1);
            }
        }
    }

    printGrid(grid);
    char choice;
    while (true) {
        std::cout << "a)nimate, t)ick, q)uit? ";
        std::cin >> choice;
        if (choice == 'a') animate(grid);
        else if (choice == 't'){
           tick(grid);
           printGrid(grid);
         }
        else return 0;
    }

}






