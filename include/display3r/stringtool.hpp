#ifndef STRINGTOOL_H
#define STRINGTOOL_H

struct StringTool {

    static bool isNumber(char c);
    static int charToInt(char c);
    static size_t numberLength(char *number);
    static float analyseNumber(char *xyz, int *i);
    static bool isLetter(char c);
    static bool isBinaryOperator(char c);

private:




    
};

#endif //STRINGTOOL_H
