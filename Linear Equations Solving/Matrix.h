#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <omp.h>
using namespace std;

template <typename T>
class Matrix
{
private:
    T* flatData;    
    T** rowPtrs;    
    int rows;
    int cols;

public:

    Matrix(int r, int c) {
        rows = r;
        cols = c;
        flatData = new T[rows * cols];
        rowPtrs = new T * [rows];

#pragma omp parallel for schedule(static)
        for (int i = 0; i < rows; i++) {
            rowPtrs[i] = &flatData[i * cols];
            for (int j = 0; j < cols; j++) {
                rowPtrs[i][j] = T(); 
            }
        }
    }

    ~Matrix() {
        delete[] rowPtrs;
        delete[] flatData;
    }

    T* operator[](int index) {
        if (index < 0 || index >= rows) exit(1);
        return rowPtrs[index];
    }


    void swapRows(int r1, int r2) {
        if (r1 == r2) return;

        // Just swap the addresses
        T* temp = rowPtrs[r1];
        rowPtrs[r1] = rowPtrs[r2];
        rowPtrs[r2] = temp;
    }

    int getTermSortID(string term) {
        if (term.empty() || term == "0") return 99999; 
        size_t xPos = term.find('x');
        if (xPos == string::npos) return 99999; 
        try {
            return stoi(term.substr(xPos + 1));
        }
        catch (...) { return 99999; }
    }



    // --- Print ---
    void print() const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << setw(10) << rowPtrs[i][j] << " ";
            }
            cout << endl;
        }
    }
};

#endif