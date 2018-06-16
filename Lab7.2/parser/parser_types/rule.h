//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_RULE_H
#define LAB7_1_RULE_H



#include <utility>
#include <vector>
#include "symbol.h"

namespace gr_parser {
    struct rule {
        symbol left;
        std::vector<symbol> right;

        rule(symbol left, std::initializer_list<symbol> right) : left(left), right(right) {}
        rule(int tag_left, std::initializer_list<symbol> symbols_right) : left(NONTERM, tag_left), right(symbols_right) {}
        rule(int left, std::vector<symbol> right) : left(NONTERM, left), right(std::move(right)) {}

        rule(const rule &other) {
            this->left = other.left;

//            for (int i = 0; i < this->right.size() && i < other.right.size(); ++i) {
//                this->right[i] = other.right[i];
//            }
            this->right = other.right;

        }
    };
}

#endif //LAB7_1_RULE_H