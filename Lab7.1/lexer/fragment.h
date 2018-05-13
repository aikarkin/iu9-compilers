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

    std::string toString() {
        std::stringstream ss;
        ss << begin.toString() << '-' << follow.toString();
        return ss.str();
    }
};

#endif //LAB7_1_FRAGMENT_H
