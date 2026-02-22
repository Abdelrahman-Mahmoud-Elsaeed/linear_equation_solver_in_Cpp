#ifndef LINEARSYSTEM_H_
#define LINEARSYSTEM_H_

#include "Matrix.h"
#include "Vector.h"
#include "Equation.h" 
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

const double EPSILON = 1e-9;

template <typename T>
class LinearSystem
{
private:
    int n;
    Matrix<T> A;
    Vector<T> B;
    Vector<T> result;
    int currentEqIndex;

public:
    LinearSystem(int size)
        : n(size),
        currentEqIndex(0),
        A(size, size),    
        B(size),          
        result(size)     
    {
    }

    ~LinearSystem() {
    }


    void addEquation(const string& input) {
        if (currentEqIndex >= n) {
            cerr << "Error: Too many equations added!" << endl;
            return;
        }

        Equation eq;
        eq.parse(input);

        B[currentEqIndex] = (T)eq.getConstant();

        Vector<Term>& terms = eq.getTerms();

        for (int i = 0; i < terms.getSize(); i++) {
            Term t = terms[i];
            int colIndex = t.index - 1;

            if (colIndex >= 0 && colIndex < n) {
                A[currentEqIndex][colIndex] += (T)t.value;
            }
        }
        currentEqIndex++;
    }

    bool solve() {
        double* bPtr = &B[0];


        for (int i = 0; i < n; i++) {

            int pivotRow = i;
            double maxVal = abs(A[i][i]);

            for (int k = i + 1; k < n; k++) {
                double val = abs(A[k][i]);
                if (val > maxVal) {
                    maxVal = val;
                    pivotRow = k;
                }
            }

            if (pivotRow != i) {
                A.swapRows(i, pivotRow);
                std::swap(bPtr[i], bPtr[pivotRow]);
            }

            if (abs(A[i][i]) < 1e-9) return false;

            double* pivotRowPtr = A[i];
            double pivotDiag = pivotRowPtr[i];

#pragma omp parallel for schedule(guided) 
            for (int k = i + 1; k < n; k++) {

                double* targetRowPtr = A[k];
                double factor = targetRowPtr[i] / pivotDiag;

                targetRowPtr[i] = 0;

                for (int j = i + 1; j < n; j++) {
                    targetRowPtr[j] -= factor * pivotRowPtr[j];
                }

                bPtr[k] -= factor * bPtr[i];
            }
        }

        // 4. Back Substitution
        for (int i = n - 1; i >= 0; i--) {
            double sum = 0;
            double* rowPtr = A[i];
            for (int j = i + 1; j < n; j++) sum += rowPtr[j] * result[j];
            result[i] = (bPtr[i] - sum) / A[i][i];
        }

        return true;
    }
    // --- Getters ---

    Matrix<T>* getMatrix() { return &A; }
    Vector<T>* getConstants() { return &B; }
    Vector<T>* getResult() { return &result; }

    int getSize() const { return n; }

    void printSolution() {
        cout << "\n--- Solution ---" << endl;
        for (int i = 0; i < n; i++) {
            cout << "x" << (i + 1) << " = " << result[i] << endl;
        }
        cout << "----------------\n" << endl;
    }
};

#endif