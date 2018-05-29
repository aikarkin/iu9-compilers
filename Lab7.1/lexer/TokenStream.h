//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_LEXER_H
#define LAB7_1_LEXER_H


#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <map>

#include "token.h"

class TokenStream {
public:
    TokenStream(std::string &source);
    Token next();
    bool hasNext();
    std::vector<std::string> &getErrors();
    void reset();
    void printDomainTags();
    std::map<std::string, int> tok_to_tag;
    std::map<int, std::string> tag_to_tok;
private:
    Position skipSpaces();
    std::string parseString();
    std::string parseNonterm();

    Position pos;
    std::string source;
    std::shared_ptr<std::vector<std::string>> errors;
    void invalidToken(Position pos, char ch);

    std::unordered_set<char> terminal_characters;
    int tag_id;
};


#endif //LAB7_1_LEXER_H
