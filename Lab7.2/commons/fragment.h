//
// Created by alex on 03.05.18.
//

#include "position.h"

#ifndef LAB7_1_FRAGMENT_H
#define LAB7_1_FRAGMENT_H

#include<sstream>

struct Fragment {
    Position begin;
    Position follow;

    Fragment() {

    }

    Fragment(const Fragment &other) {
        this->begin = Position(other.begin);
        this->follow = Position(other.follow);
    }

    std::string str() {
        return begin.str() + "-" + follow.str();
    }

    std::string token() {
        return follow.src.substr(begin.offset, follow.offset - begin.offset);
    }
};

#endif //LAB7_1_FRAGMENT_H
