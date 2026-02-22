#ifndef COMMAND_H_
#define COMMAND_H_

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "LinearSystem.h"

using namespace std;

class Command
{
private:
    LinearSystem<double>* sys;

    // --- Helper: Print Row ---
    void printRow(int rowIndex) {
        Matrix<double>* A = sys->getMatrix();
        Vector<double>* B = sys->getConstants();
        int n = sys->getSize();

        // Limit output for massive systems
        int limit = (n > 20) ? 20 : n;

        for (int j = 0; j < limit; j++) {
            double val = (*A)[rowIndex][j];
            if (val == 0) continue; // Skip zero terms for readability

            if (j > 0 && val >= 0) cout << "+";
            cout << val << "x" << (j + 1) << " ";
        }

        if (n > 20) cout << "... ";
        cout << "= " << (*B)[rowIndex] << endl;
    }

    // --- Helper: Calculate Determinant (On Demand Only) ---
    // Optimization: This creates a copy, solves it, returns result.
    // It does NOT affect the main 'sys' matrix.
    double calculateDeterminant() {
        int n = sys->getSize();
        Matrix<double>* mainMat = sys->getMatrix();

        // 1. Create a temporary matrix to destroy
        Matrix<double> temp(n, n);

        // 2. Fast Copy
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                temp[i][j] = (*mainMat)[i][j];
            }
        }

        // 3. Gaussian Elimination (on temp)
        double det = 1.0;
        for (int i = 0; i < n; i++) {
            int pivot = i;
            // Find pivot
            for (int k = i + 1; k < n; k++) {
                if (abs(temp[k][i]) > abs(temp[pivot][i])) pivot = k;
            }

            // Swap
            if (pivot != i) {
                temp.swapRows(i, pivot);
                det *= -1;
            }

            // Zero Check
            if (abs(temp[i][i]) < 1e-9) return 0;

            det *= temp[i][i];

            // Eliminate
            for (int k = i + 1; k < n; k++) {
                double factor = temp[k][i] / temp[i][i];
                for (int j = i; j < n; j++) {
                    temp[k][j] -= factor * temp[i][j];
                }
            }
        }
        return det;
    }

public:
    Command(LinearSystem<double>* system) : sys(system) {}

    void run() {
        string line;
        cout << "Command Interface Ready. Type 'help' for a list of commands, or 'exit' to quit." << endl;

        while (true) {
            cout << "> ";
            getline(cin, line);
            if (line.empty()) continue;

            stringstream ss(line);
            string cmd;
            ss >> cmd;

            if (cmd == "exit") break;

            // --- HELP COMMAND ---
            if (cmd == "help") {
                cout << "\n--- Available Commands ---\n"
                    << left << setw(35) << "help" << "- Show this help message\n"
                    << left << setw(35) << "exit" << "- Exit the command interface\n"
                    << left << setw(35) << "num_vars" << "- Display the number of variables in the system\n"
                    << left << setw(35) << "equation <index>" << "- Display the equation at the specified index (1-based)\n"
                    << left << setw(35) << "column <var_name>" << "- Display the coefficients of a specific variable (e.g., column x1)\n"
                    << left << setw(35) << "add <target> <source>" << "- Add source equation to target equation (modifies target)\n"
                    << left << setw(35) << "subtract <target> <source>" << "- Subtract source eq from target eq (modifies target)\n"
                    << left << setw(35) << "substitute <var> <tgt> <src>" << "- Eliminate <var> in <tgt> equation using <src> equation\n"
                    << left << setw(35) << "D" << "- Display the current state of the matrix/system\n"
                    << left << setw(35) << "D_value" << "- Calculate and display the determinant of the matrix\n"
                    << left << setw(35) << "solve" << "- Solve the linear system and display the result\n"
                    << "--------------------------\n";
            }
            // --- LEVEL 1: Basic View ---
            else if (cmd == "num_vars") {
                cout << sys->getSize() << endl;
            }
            else if (cmd == "equation") {
                int idx;
                if (ss >> idx) {
                    if (idx < 1 || idx > sys->getSize()) cout << "Invalid index.\n";
                    else printRow(idx - 1);
                }
            }
            else if (cmd == "column") {
                string varName;
                ss >> varName;
                size_t xPos = varName.find('x');
                if (xPos != string::npos) {
                    try {
                        int colIdx = stoi(varName.substr(xPos + 1)) - 1;
                        Matrix<double>* A = sys->getMatrix();
                        if (colIdx >= 0 && colIdx < sys->getSize()) {
                            // Limit output
                            int limit = (sys->getSize() > 20) ? 20 : sys->getSize();
                            for (int i = 0; i < limit; i++) cout << (*A)[i][colIdx] << endl;
                            if (sys->getSize() > 20) cout << "..." << endl;
                        }
                    }
                    catch (...) { cout << "Invalid column format." << endl; }
                }
            }

            // --- LEVEL 2: Row Operations ---
            else if (cmd == "add" || cmd == "subtract") {
                int i, j;
                if (ss >> i >> j) {
                    Matrix<double>* A = sys->getMatrix();
                    Vector<double>* B = sys->getConstants();
                    int r1 = i - 1;
                    int r2 = j - 1;
                    int n = sys->getSize();

                    for (int k = 0; k < n; k++) {
                        if (cmd == "add") (*A)[r1][k] += (*A)[r2][k];
                        else              (*A)[r1][k] -= (*A)[r2][k];
                    }
                    if (cmd == "add") (*B)[r1] += (*B)[r2];
                    else              (*B)[r1] -= (*B)[r2];

                    printRow(r1);
                }
            }
            else if (cmd == "substitute") {
                string varName;
                int targetEq, srcEq;
                if (ss >> varName >> targetEq >> srcEq) {
                    try {
                        int colIdx = stoi(varName.substr(varName.find('x') + 1)) - 1;
                        int tIdx = targetEq - 1;
                        int sIdx = srcEq - 1;

                        Matrix<double>* A = sys->getMatrix();
                        Vector<double>* B = sys->getConstants();

                        double pivot = (*A)[sIdx][colIdx];
                        if (abs(pivot) < 1e-9) {
                            cout << "Cannot substitute: Pivot is zero." << endl;
                        }
                        else {
                            double factor = (*A)[tIdx][colIdx] / pivot;

                            for (int k = 0; k < sys->getSize(); k++) {
                                (*A)[tIdx][k] -= factor * (*A)[sIdx][k];
                            }
                            (*B)[tIdx] -= factor * (*B)[sIdx];

                            (*A)[tIdx][colIdx] = 0.0;

                            printRow(tIdx);
                        }
                    }
                    catch (...) { cout << "Invalid syntax." << endl; }
                }
            }

            // --- LEVEL 3: Determinant & Solving ---
            else if (cmd == "D") {
                cout << "Displaying matrix (limited view)..." << endl;
                sys->printSolution();
            }
            else if (cmd == "D_value") {
                cout << "Calculating Determinant (this may take a moment)..." << endl;
                cout << "Determinant: " << calculateDeterminant() << endl;
            }
            else if (cmd == "solve") {
                cout << "Solving system..." << endl;
                if (sys->solve()) {
                    sys->printSolution();
                }
                else {
                    cout << "System has no unique solution." << endl;
                }
            }
            else {
                cout << "Unknown command. Type 'help' to see available commands." << endl;
            }
        }
    }
};

#endif