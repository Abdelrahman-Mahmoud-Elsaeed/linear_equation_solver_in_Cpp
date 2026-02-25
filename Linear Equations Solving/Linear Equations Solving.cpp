#include "LinearSystem.h"
#include "Command.h"
#include "EquationGenerator.h"
#include <omp.h> 
#include <chrono>
using namespace std;

int main() {
    int mode;
    cout << "Select mode:\n"
        << " 1. Normal (user input + command interface)\n"
        << " 2. Benchmark (generation / timing)\n"
        << "Choice: ";
    cin >> mode;
    cin.ignore();

    if (mode == 2) {
        // --- benchmark mode ---
        int n;
        int choice;

        cout << "--- Serial Linear System Solver (Benchmark) ---\n";
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
            // Manual Input
            cout << "Enter " << n << " equations:" << endl;
            for (int i = 0; i < n; i++) {
                string line;
                getline(cin, line);
                sys.addEquation(line);
            }
        }

        // --- SOLVE ---
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
        cout << "\nPress Enter to exit...";
        cin.get();
        return 0;
    }
    else {
        // --- normal interactive mode ---
        int n;
        cout << "Enter number of variables: ";
        cin >> n;
        cin.ignore();

        LinearSystem<double> sys(n);

        cout << "Enter " << n << " equations (e.g. 3x1 + 4x2 = 9):" << endl;
        for (int i = 0; i < n; i++) {
            string line;
            getline(cin, line);
            sys.addEquation(line);
        }

        Command cmdInterface(&sys);
        cmdInterface.run();

        cout << "\nPress Enter to exit...";
        cin.get();
        return 0;
    }
};



