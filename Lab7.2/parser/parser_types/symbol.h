//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_RULEITEM_H
#define LAB7_1_RULEITEM_H

#include "symbol_type.h"
#include "../../lexer/Lexer.h"

namespace gr_parser {

    struct symbol {
        symbol_t type;
        int tag;

        symbol() : type(TERM), tag(-1) {}

        symbol(symbol_t type, int tag) : type(type), tag(tag) {}

        symbol(const symbol& other) {
            this->type = other.type;
            this->tag = other.tag;
        }
    };
}

#endif //LAB7_1_RULEITEM_H