//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_PARSER_H
#define LAB7_1_PARSER_H


#include "parser_types/rule.h"
#include "../tree/tree.hpp"
#include "../commons/token.h"
#include "../lexer/Lexer.h"
#include <stack>
#include <algorithm>
#include <variant>

namespace gr_parser {
    using st_data = std::variant<std::monostate, int, Token>;
    using st_iter = stl_tree::tree<st_data>::iterator;

    class Parser {
    public:
        Parser(std::vector<std::vector<int>> transitions, std::vector<rule> rules);

        stl_tree::tree<st_data> parse(gr_lexer::TokenStream tokens);

        std::vector<std::string> &getErrors();

    private:
        std::stack<symbol> symbols;

        std::vector<std::vector<int>> transitions;
        std::vector<rule> rules;
        std::stack<st_iter> nodes;

        std::vector<std::string> errors;

        int goTo(int nonterm, int tag);

        std::string invalidToken(Token tok);

        void printStack();
    };

};

#endif