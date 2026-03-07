#include "LinearSystem.h"
#include "Command.h"
#include "EquationGenerator.h"
#include <omp.h> 
#include <chrono>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

void runTest(int testNum, const string& desc, int vars, const vector<string>& equations) {
    cout << "========================================\n";
    cout << "Test " << testNum << ": " << desc << "\n";
    cout << "========================================\n";

    LinearSystem<double> sys(vars);
    bool valid = true;

    for (const string& eq : equations) {
        cout << "Input: " << eq << "\n";
        if (!sys.addEquation(eq)) {
            cout << "-> Failed to add equation.\n";
            valid = false;
        }
    }

    if (valid) {
        if (sys.solve()) {
            sys.printSolution();
        }
        else {
            cout << "\n[Result] Singular matrix: No unique solution (Infinite or No Solution).\n\n";
        }
    }
    else {
        cout << "\n[Result] System solving aborted due to invalid input.\n\n";
    }
}

int main() {
    int mode;
    cout << "Select mode:\n"
        << " 1. Normal (user input + command interface)\n"
        << " 2. Benchmark (generation / timing)\n"
        << " 3. Run Automated Tests (10 Cases)\n"
        << "Choice: ";
    cin >> mode;
    cin.ignore();

    if (mode == 2) {
        int n;
        int choice;

        cout << "\n--- Serial Linear System Solver (Benchmark) ---\n";
        cout << "Enter number of variables (N): ";
        cin >> n;

        LinearSystem<double> sys(n);

        cout << "\nChoose Input Method:\n";
        cout << "1. Manual Input\n";
        cout << "2. Stream Auto-Generate (Memory Efficient)\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 2) {
            EquationGenerator gen;
            cout << "Streaming " << n << " equations (Generate -> Add)..." << endl;

            auto start = std::chrono::high_resolution_clock::now();

            for (int i = 0; i < n; i++) {
                string eq = gen.generateMixedEquation(n);
                sys.addEquation(eq);
            }

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            cout << "Generation & Parsing Time: " << diff.count() << " seconds." << endl;
        }
        else {
            cout << "Enter " << n << " equations:" << endl;
            for (int i = 0; i < n; ) {
                string line;
                cout << "Eq " << (i + 1) << ": ";
                getline(cin, line);

                if (sys.addEquation(line)) {
                    i++;
                }
                else {
                    cout << "Please try entering equation " << (i + 1) << " again.\n";
                }
            }
        }

        cout << "-----------------------------------" << endl;
#ifdef _OPENMP
        cout << "Parallel Mode: ON" << endl;
        cout << "Maximum Threads Available: " << omp_get_max_threads() << endl;
#else
        cout << "Parallel Mode: OFF (Compiler ignored it!)" << endl;
#endif
        cout << "-----------------------------------" << endl;

        auto startSolve = std::chrono::high_resolution_clock::now();
        bool success = sys.solve();
        auto endSolve = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> diffSolve = endSolve - startSolve;

        if (success) {
            cout << "System Solved in " << diffSolve.count() << " seconds." << endl;
            if (n <= 100) sys.printSolution();
        }
        else {
            cout << "System could not be solved (Singular Matrix / No unique solution)." << endl;
        }

        cout << "\nPress Enter to exit...";
        cin.get();
        return 0;
    }
    else if (mode == 3) {
        cout << "\n--- Running Automated Test Suite ---\n\n";

        runTest(1, "Standard 2x2 System", 2, { "2x1 + 1x2 = 5", "-1x1 + 1x2 = 2" });
        runTest(2, "Implicit Coefficients", 2, { "x1 + x2 = 5", "x1 - x2 = 1" });
        runTest(3, "Unordered Variables", 2, { "4x2 + 3x1 = 10", "x1 - x2 = 1" });
        runTest(4, "Missing Variables", 2, { "x1 = 5", "2x1 + 3x2 = 16" });
        runTest(5, "Decimals and Negatives", 2, { "-0.5x1 + 1.5x2 = 3.5", "2.2x1 - 1.1x2 = 0" });

        runTest(6, "Inconsistent System (No Solution)", 2, { "x1 + x2 = 5", "x1 + x2 = 10" });
        runTest(7, "Dependent System (Infinite Solutions)", 2, { "x1 + x2 = 5", "2x1 + 2x2 = 10" });

        runTest(8, "Missing Equals Sign", 2, { "3x1 + 4x2 9", "x1 - x2 = 1" });
        runTest(9, "Multiple Equals Signs", 2, { "3x1 + 4x2 == 9", "x1 - x2 = 1" });
        runTest(10, "Invalid Characters", 2, { "3x1 + a*x2 = 9", "x1 - x2 = 1" });

        cout << "\nPress Enter to exit...";
        cin.get();
        return 0;
    }
    else {
        int n;
        cout << "Enter number of variables: ";
        cin >> n;
        cin.ignore();

        LinearSystem<double> sys(n);

        cout << "Enter " << n << " equations (e.g. 3x1 + 4x2 = 9):" << endl;

        for (int i = 0; i < n; ) {
            string line;
            cout << "Eq " << (i + 1) << ": ";
            getline(cin, line);

            if (sys.addEquation(line)) {
                i++;
            }
            else {
                cout << "Please try entering equation " << (i + 1) << " again.\n\n";
            }
        }

        Command cmdInterface(&sys);
        cmdInterface.run();

        cout << "\nPress Enter to exit...";
        cin.get();
        return 0;
    }
}