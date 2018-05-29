//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_RULE_H
#define LAB7_1_RULE_H



#include <utility>
#include <vector>
#include "symbol.h"

struct rule {
    symbol left;
    std::vector<symbol> right;

    rule(symbol left, std::initializer_list<symbol> right) : left(left), right(right) { }
    rule(int tag_left, std::initializer_list<symbol> symbols_right) : left(NONTERM, tag_left), right(symbols_right) {}
    rule(const rule& other) {
        this->left = other.left;
        this->right = other.right;
    }
};

#endif //LAB7_1_RULE_H