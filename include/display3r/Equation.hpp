#ifndef EQUATION_H
#define EQUATION_H

#include <string>
#include <vector>
#include <stack>

// #include "display3r/state.hpp"
// #include "display3r/point.hpp"
// #include "display3r/tree.hpp"

class Equation {
public:
    Equation(std::string eqpath_) {}

    // static int analyseBinaryOperator(char *xyz, int *i);
    // static int initEquation(float *minS, float *maxS, int *precisionS,
    //     		    float *minT, float *maxT, int *precisionT,
    //     		    char *x, char *y, char *z, int strSize,
    //     		    std::string eqpath);

    // Point getPointFromEquation(float s, float t);

    // Tree parseLine(std::string);

    // private:
    // State state;
    // std::stack<void*> stack;
    // float minsS, maxS, minT, maxT;
    // float precisionS, precisionT;

    // struct Token {
    //     bool isOpeningParen();
    //     bool isClosingParen();
    // };


    // std::string eqpath;
    // std::string xeql, yeql, zeql;
    // Tree xeq, yeq, zeq;

    // const int VARS[2] = { 't', 's' };

    // enum {FUNCTION, OPERATOR, NUMBER};
    // enum {COS, SIN, SQRT, SQR};
    // enum {PLUS, MINUS, TIMES, OVER};
    // enum {POTENTIAL, ACCEPT, REFUSE};

    // static bool isVariable(char *xyz, int i);
    // static int analyseFunction(char *xyz, int *i);
    // static float analyseVariable(char c, float s, float t);
    // static void handleFunction(int id, char xyz, char f,
    //     		       int fFinalCharId, int j);
    // static float computeOperator(float x, int op, float y);
    // static float computeFunction(int function, float x);
    // float getValue(Tree tr, float s, float t);
};

#endif //EQUATION_H
