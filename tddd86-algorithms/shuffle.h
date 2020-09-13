/*
 * TDDD86 Boggle
 * This file contains implementation of a shuffling function that operates on
 * a 1-D and 2-D array, vector, or Grid of any type.
 * You can use it in your program.
 * Please do not modify this provided file.
 */

#ifndef _shuffle_h
#define _shuffle_h

#include "grid.h"
#include "random.h"
#include <vector>

template <typename T>
void shuffle(T* array, int length) {
    for (int i = 0; i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            T temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
}

template <typename T>
void shuffle(T** array2d, int rows, int cols) {
    int length = rows * cols;
    for (int i = 0; i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            int r1 = i / cols;
            int c1 = i % cols;
            int r2 = j / cols;
            int c2 = j % cols;

            T temp = array2d[r1][c1];
            array2d[r1][c1] = array2d[r2][c2];
            array2d[r2][c2] = temp;
        }
    }
}

template <typename T>
void shuffle(vector<T>& v) {
    for (int i = 0, length = v.size(); i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            T temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }
}

template <typename T>
void shuffle(Grid<T>& grid) {
    int rows = grid.numRows();
    int cols = grid.numCols();
    int length = rows * cols;
    for (int i = 0; i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            int r1 = i / cols;
            int c1 = i % cols;
            int r2 = j / cols;
            int c2 = j % cols;

            T temp = grid[r1][c1];
            grid[r1][c1] = grid[r2][c2];
            grid[r2][c2] = temp;
        }
    }
}

#endif
