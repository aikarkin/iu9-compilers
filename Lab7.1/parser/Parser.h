//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_PARSER_H
#define LAB7_1_PARSER_H


#include "../lexer/TokenStream.h"
#include "parser_types/rule.h"
#include "../tree/tree.hpp"
#include <stack>
#include <algorithm>
#include <variant>

struct st_data {
    std::variant<std::monostate, int, Token> data;
    int childrenCount;

    st_data() {
        childrenCount = 0;
    }

    st_data(int nonTermTag) {
        data = nonTermTag;
        childrenCount = 0;
    }

    st_data(Token tok) {
        data = tok;
        childrenCount = 0;
    }
};

using st_iter = stl_tree::tree<st_data>::iterator;

class Parser {
public:
    Parser(TokenStream tokens);
    stl_tree::tree<st_data> parse();
    std::vector<std::string> &getErrors();
private:
    std::stack<symbol> symbols;

    std::vector<std::vector<int>> transitions;
    std::vector<rule> rules;
    std::stack<st_iter> nodes;

    TokenStream tokens;
    std::vector<std::string> errors;

    int goTo(int nonterm, int tag);
    std::string invalidToken(Token tok);
    void printStack();
};


#endif