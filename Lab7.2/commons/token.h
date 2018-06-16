//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_TOKEN_H
#define LAB7_1_TOKEN_H

#include <utility>
#include "fragment.h"
#include "../parser/parser_types/symbol.h"

struct Token {
    Fragment frag;
    int tag;
    std::optional<std::string> value;

    Token() { }

    std::string str() {
     //   return "Tag: " + std::to_string(tag) + " "  + frag.str() + ": " + (value.has_value() ? value.value() : "");
        return "[" + std::to_string(tag) + "] '" + (value.has_value() ? value.value() : "") + "'";
    }
};

#endif //LAB7_1_TOKEN_H
