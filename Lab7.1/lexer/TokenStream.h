//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_LEXER_H
#define LAB7_1_LEXER_H


#include <string>
#include <vector>
#include <memory>
#include "token.h"

class TokenStream {
public:
    TokenStream(std::string &source);
    Token next();
    bool hasNext();
    std::vector<std::string> &getErrors();
private:
    Position skipSpaces();
    Position pos;
    std::string source;
    std::shared_ptr<std::vector<std::string>> errors;
    void invalidToken(Position pos, char ch);
};


#endif //LAB7_1_LEXER_H
