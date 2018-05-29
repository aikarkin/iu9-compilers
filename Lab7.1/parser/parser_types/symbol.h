//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_RULEITEM_H
#define LAB7_1_RULEITEM_H

#include "symbol_type.h"



struct symbol {
    symbol_t type;
    int tag;

    symbol() : type(TERM), tag(EMPTY) {}
    symbol(symbol_t type, int tag) : type(type), tag(tag) {}
};

#endif //LAB7_1_RULEITEM_H