#include <iostream>
#include <string>
#include <random>
#include <sstream>
#include <chrono>


using namespace std;

class EquationGenerator {
private:
    int getRand(int min, int max) {
        static std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }

public:

    string generateMixedEquation(int numVars) {
        ostringstream ss;

        int density = (numVars > 10) ? numVars / 2 : numVars;

        for (int i = 0; i < density; i++) {
            int coeff = getRand(-100, 100);
            int varIdx = getRand(1, numVars); // 1-based index

            if (coeff == 0) continue;
            if (i > 0 && coeff > 0) ss << "+";

            ss << coeff << "x" << varIdx;
        }

        ss << "=";

        ss << getRand(-500, 500);

        return ss.str();
    }
};