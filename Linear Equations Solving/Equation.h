#ifndef EQUATION_H_
#define EQUATION_H_

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "Vector.h"

using namespace std;

struct Term {
    int index;
    double value;
};

class Equation
{
private:
    Vector<Term> terms;
    double constant;

    bool isDigit(char c) {
        return (c >= '0' && c <= '9');
    }

    void addOrUpdateTerm(int index, double val) {
        int pos = index - 1;

        if (pos < 0) return;

        while (terms.getSize() <= pos) {
            terms.push({ terms.getSize() + 1, 0.0 });
        }

        terms[pos].value += val;
        terms[pos].index = index;
    }

    void parseTerm(string token, bool isRHS) {
        if (token.empty()) return;

        size_t xPos = token.find('x');

        if (xPos != string::npos) {
            string coeffStr = token.substr(0, xPos);
            double coeff = (coeffStr.empty() || coeffStr == "+") ? 1.0 :
                (coeffStr == "-") ? -1.0 : stod(coeffStr);

            if (xPos + 1 < token.length()) {
                int index = stoi(token.substr(xPos + 1));

                if (isRHS) coeff = -coeff;

                addOrUpdateTerm(index, coeff);
            }
        }
        else {
            double val = stod(token);

            if (!isRHS) val = -val;

            constant += val;
        }
    }
public:

    Equation() : terms(0), constant(0) {}

    bool parse(string line) {
        int eqCount = 0;
        for (char c : line) {
            if (c == '=') {
                eqCount++;
            }
            if (!isDigit(c) && c != ' ' && c != '+' && c != '-' &&
                c != '=' && c != '.' && c != 'x') {

                cout << "\n[Error] Invalid character detected: '" << c << "'\n"
                    << "Only numbers, standard variables (x1, x2), and operators are allowed.\n";
                return false;
            }
        }

        if (eqCount == 0) {
            cout << "\n[Error] Invalid format: Missing '=' sign in equation.\n";
            return false;
        }
        if (eqCount > 1) {
            cout << "\n[Error] Invalid format: Multiple '=' signs detected.\n";
            return false;
        }

        string leftPart, rightPart;
        size_t eqPos = line.find('=');
        leftPart = line.substr(0, eqPos);
        rightPart = line.substr(eqPos + 1);

        auto tokenize = [&](const string& s, bool isRight) {
            string currentTerm;
            currentTerm.reserve(16);

            for (char c : s) {
                if (c == ' ') continue;

                if ((c == '+' || c == '-') && !currentTerm.empty()) {
                    parseTerm(currentTerm, isRight);
                    currentTerm.clear();
                }

                currentTerm += c;
            }

            if (!currentTerm.empty()) {
                parseTerm(currentTerm, isRight);
            }
            };

        try {
            tokenize(leftPart, false);
            tokenize(rightPart, true);
        }
        catch (const exception& e) {
            cout << "\n[Error] Failed to parse terms. Please check your math syntax.\n";
            return false; 
        }

        return true;
    }

    Vector<Term>& getTerms() { return terms; }

    double getConstant() const { return constant; }

    void print() {
        cout << "Terms: ";

        for (int i = 0; i < terms.getSize(); i++) {
            Term t = terms[i];
            cout << t.value << "x" << t.index << " ";
        }
        cout << " | Constant: " << constant << endl;
    }
};

#endif