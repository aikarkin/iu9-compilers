//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_PARSER_H
#define LAB7_1_PARSER_H


#include "../lexer/TokenStream.h"
#include "Rule.h"
#include "../tree/tree.hpp"
#include <stack>
#include <algorithm>

class Parser {
public:
    Parser(TokenStream &tokens);
    void parse();
    std::vector<std::string> &getErrors();
    stl_tree::tree<RuleItem> getSyntaxTree();
private:
    std::stack<RuleItem> items;

    std::vector<Rule> rules;
    std::vector<std::string> errors;
    TokenStream tokens;
    std::vector<std::vector<int>> transitions;
    std::vector<int> result;

    int goTo(NonTerm nonterm, DomainTag tag);
//    void printStack(std::stack<RuleItem> stack);
};


#endif