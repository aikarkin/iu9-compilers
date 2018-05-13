//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_TOKEN_H
#define LAB7_1_TOKEN_H

#include "domain_tags.h"
#include "fragment.h"
#include "../parser/RuleItem.h"

struct Token {
    DomainTag tag;
    Fragment frag;

    Token(Fragment frag, DomainTag tag) : tag(tag), frag(frag) {};

    std::string toString() {
        std::stringstream ss;

        ss << TAG_NAMES[tag];

        if(tag != EOP)
            ss << " " << frag.toString() << ":";

        return ss.str();
    }
};

#endif //LAB7_1_TOKEN_H
