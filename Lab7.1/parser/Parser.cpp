//
// Created by alex on 03.05.18.
//

#include <iostream>
#include "Parser.h"


Parser::Parser(TokenStream &tokens) : tokens(tokens), errors(), result() {
    rules = std::vector<Rule>{
       /* 0 */   Rule(E,  {T, E1}),
       /* 1 */   Rule(E1, {ADD, T, E1}),
       /* 2 */   Rule(E1, {EPS}),
       /* 3 */   Rule(T,  {F, T1}),
       /* 4 */   Rule(T1, {MUL, F, T1}),
       /* 5 */   Rule(T1, {EPS}),
       /* 6 */   Rule(F,  {NUM}),
       /* 7 */   Rule(F,  {LPAREN, E, RPAREN}),
    };


    this->transitions = std::vector<std::vector<int>> {
         /*               +       *       n       (       )       $       */
         /*  E */   {    -1,     -1,      0,      0,     -1,     -1     },
         /* E1 */   {     1,     -1,     -1,     -1,      2,      2     },
         /*  T */   {    -1,     -1,      3,      3,     -1,     -1     },
         /* T1 */   {     5,      4,     -1,     -1,      5,      5     },
         /*  F */   {    -1,     -1,      6,      7,     -1,     -1     },
    };
}

std::vector<std::string> &Parser::getErrors() {
    return this->errors;
}


int Parser::goTo(NonTerm nonterm, DomainTag term) {
    if(nonterm >= transitions.size() || term >= transitions.at(nonterm).size() + 1) {
        return -1;
    } else if(term == EOP) {
        return transitions.at(nonterm).at(EPS);
    }

    return transitions.at(nonterm).at(term);
}

void Parser::parse() {
    if(!tokens.hasNext())
        return;

    Token tok = tokens.next();
    this->items.push(EOP);
    this->items.push(E);

    do {
        RuleItem top = this->items.top();
        if(top.type == TERM) {
            if (top.value.term == tok.tag) {
                this->items.pop();

                if (tokens.hasNext()) {
                    tok = tokens.next();
                } else {
                    break;
                }
            } else if(top.value.term == EPS) {
                this->items.pop();
            } else {
                errors.push_back("Syntax error at " + tok.frag.begin.toString() + ": expected " + top.toString() +
                                 ", but \"" + TAG_NAMES[tok.tag] + "\" found");
                if(tokens.hasNext()) {
                    tok = tokens.next();
                } else {
                    break;
                }
            }
        } else {
            int ruleIdx;
            if((ruleIdx = goTo(top.value.nonterm, tok.tag)) >= 0) {
                items.pop();

                for (int i = (int)rules[ruleIdx].right.size() - 1; i >= 0; --i) {
                    items.push(rules[ruleIdx].right[i]);
                }
                result.push_back(ruleIdx);
            } else {
                if(tok.tag == EOP) {
                    errors.emplace_back("Syntax error: unexpected end of program");
                } else {
                    errors.emplace_back("Syntax error at " + tok.frag.begin.toString() + ": unexpected token \"" +
                                        TAG_NAMES[tok.tag] + "\"");
                }
                if(tokens.hasNext())
                    tok = tokens.next();
                else {
                    break;
                }
            }
        }
    } while(!items.empty());

    while(!items.empty()) {
        int ruleIdx;
        RuleItem top = items.top();
        if((ruleIdx = goTo(top.value.nonterm, tok.tag)) >= 0) {
            result.push_back(ruleIdx);
        }
        items.pop();
    }
}

stl_tree::tree<RuleItem> Parser::getSyntaxTree() {
    using iter = stl_tree::tree<RuleItem>::iterator;
    stl_tree::tree<RuleItem> tr(rules[result[0]].left);
    std::stack<iter> nodes;
    nodes.push(tr.begin());


    for(int ri : result) {
        Rule rule = rules[ri];
        iter top = nodes.top();
        nodes.pop();

        for (int i = (int)rule.right.size() - 1; i >= 0; --i) {
            iter appended = tr.append_child(top, rule.right[i]);
            if(rule.right[i].type == NONTERM)
                nodes.push(appended);
        }
    }


    return tr;
}


//void Parser::printStack(std::stack<RuleItem> stack) {
//    std::vector<std::string> items;
//    std::cout << "stack: ";
//
//    while(!stack.empty()) {
//        items.push_back(stack.top().toString());
//        stack.pop();
//    }
//
//    std::reverse(items.begin(), items.end());
//
//    for(std::string &item : items) {
//        std::cout << item << " ";
//    }
//
//    std::cout << std::endl;
//}
