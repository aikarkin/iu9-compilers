//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_RULEITEM_H
#define LAB7_1_RULEITEM_H



#include "../lexer/domain_tags.h"
#include "NonTerm.h"

enum Type {
    TERM,
    NONTERM
};



struct RuleItem {
    Type type;

    union ItemValue {
        DomainTag term;
        NonTerm nonterm;
    } value;


    RuleItem(DomainTag term) {
        this->value.term = term;
        this->type = TERM;
    }

    RuleItem(NonTerm nonterm) {
        this->value.nonterm = nonterm;
        this->type = NONTERM;
    }

    RuleItem() {}
    
    std::string toString() {
        return type == TERM ? "\"" + TAG_NAMES[value.term] + "\"" : NONTERM_NAMES[value.nonterm];

    }
};

#endif //LAB7_1_RULEITEM_H