//
// Created by alex on 24.05.18.
//

#ifndef LAB7_1_SYNTAX_TREE_NODE_H
#define LAB7_1_SYNTAX_TREE_NODE_H

#include <string>
#include "symbol_type.h"
#include "../../lexer/token.h"
#include <variant>

struct st_data {
    std::variant<std::string, Token> val;
    symbol_t type;
    int tag;

    st_data(int tag, std::string nonTerm) : type(NONTERM), tag(tag), val(nonTerm) { }

    explicit st_data(Token tok) : type(TERM), tag(tok.tag), val(tok) { }
};

#endif //LAB7_1_SYNTAX_TREE_NODE_H
