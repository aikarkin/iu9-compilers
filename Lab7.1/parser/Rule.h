//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_RULE_H
#define LAB7_1_RULE_H



#include <utility>
#include <vector>
#include "RuleItem.h"

struct Rule {
    NonTerm left;
    std::vector<RuleItem> right;

    Rule(NonTerm left, std::initializer_list<RuleItem> right) : left(left), right(right) { }
};

#endif //LAB7_1_RULE_H