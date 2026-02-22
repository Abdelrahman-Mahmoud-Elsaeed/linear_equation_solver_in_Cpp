#ifndef GAUSSIANSOLVER_H_
#define GAUSSIANSOLVER_H_

#include "LinearSystem.h"
#include "Matrix.h"
#include "Vector.h"
#include <cmath> // for std::abs

using namespace std;

template <typename T>
class GaussianSolver
{
public:
    void solve(LinearSystem<T>* sys) {

        Matrix<T>* A = sys->getMatrix();
        Vector<T>* B = sys->getConstants();
        Vector<T>* X = sys->getResult(); 

        int n = sys->getSize();

        for (int k = 0; k < n; k++) {

            int maxRow = k;
            for (int i = k + 1; i < n; i++) {
                if (abs((*A)[i][k]) > abs((*A)[maxRow][k])) {
                    maxRow = i;
                }
            }


            for (int j = k; j < n; j++) {
                T temp = (*A)[k][j];
                (*A)[k][j] = (*A)[maxRow][j];
                (*A)[maxRow][j] = temp;
            }

            T tempB = (*B)[k];
            (*B)[k] = (*B)[maxRow];
            (*B)[maxRow] = tempB;


            if (abs((*A)[k][k]) < 1e-9) {
                continue;
            }

            for (int i = k + 1; i < n; i++) {
                double factor = (*A)[i][k] / (*A)[k][k];

                (*B)[i] -= factor * (*B)[k];


                (*A)[i][k] = 0; 
                for (int j = k + 1; j < n; j++) {
                    (*A)[i][j] -= factor * (*A)[k][j];
                }
            }
        }


        for (int i = n - 1; i >= 0; i--) {
            double sum = 0;

            for (int j = i + 1; j < n; j++) {
                sum += (*A)[i][j] * (*X)[j];
            }

            (*X)[i] = ((*B)[i] - sum) / (*A)[i][i];
        }
    }
};

#endif