//
// Created by alex on 03.05.18.
//

#include <iostream>
#include <utility>
#include "Parser.h"


Parser::Parser(TokenStream tokens): tokens(std::move(tokens)), errors()  {
    this->rules =  std::vector<rule> {
            rule(0, { symbol(NONTERM, 1), symbol(NONTERM, 2) }),
            rule(1, { symbol(TERM, 1), symbol(TERM, 2), symbol(TERM, 3), symbol(NONTERM, 3) }),
            rule(3, { symbol(TERM, 4), symbol(TERM, 2), symbol(NONTERM, 3) }),
            rule(3, { symbol(TERM, -1) }),
            rule(2, { symbol(TERM, 5), symbol(NONTERM, 4), symbol(TERM, 7), symbol(NONTERM, 2) }),
            rule(2, { symbol(TERM, -1) }),
            rule(4, { symbol(TERM, 2), symbol(TERM, 6), symbol(NONTERM, 5), symbol(NONTERM, 6) }),
            rule(6, { symbol(TERM, 6), symbol(NONTERM, 5), symbol(NONTERM, 6) }),
            rule(6, { symbol(TERM, -1) }),
            rule(5, { symbol(NONTERM, 7), symbol(NONTERM, 8) }),
            rule(8, { symbol(NONTERM, 7), symbol(NONTERM, 8) }),
            rule(8, { symbol(TERM, -1) }),
            rule(7, { symbol(TERM, 8) }),
            rule(7, { symbol(TERM, 2) }),
            rule(7, { symbol(TERM, 9) }),
    };


    this->transitions = std::vector<std::vector<int>> {
            /*                (0)     (1)      (2)     (3)     (4)     (5)     (6)     (7)       (8)      (9)                */
            /*                  $       {  nonterm       }       ,       [       :       ]      term       @
            /*  S (0)  */    { -1,      0,      -1,     -1,     -1,     -1,     -1,     -1,       -1,     -1 },
            /*  A (1)  */    { -1,      1,      -1,     -1,     -1,     -1,     -1,     -1,       -1,     -1 },
            /*  B (2)  */    {  5,     -1,      -1,     -1,     -1,      4,     -1,     -1,       -1,     -1 },
            /*  C (3)  */    {  3,     -1,      -1,     -1,      2,      3,     -1,     -1,       -1,     -1 },
            /*  D (4)  */    { -1,     -1,       6,     -1,     -1,     -1,     -1,     -1,       -1,     -1 },
            /*  F (5)  */    { -1,     -1,       9,     -1,     -1,     -1,     -1,     -1,        9,      9 },
            /*  E (6)  */    { -1,     -1,      -1,     -1,     -1,     -1,      7,      8,       -1,     -1 },
            /*  G (7)  */    { -1,     -1,      13,     -1,     -1,     -1,     -1,     -1,       12,     14 },
            /* F1 (8)  */    { -1,     -1,      10,     -1,     -1,     -1,     11,     11,       10,     10 },
    };
}

std::vector<std::string> &Parser::getErrors() {
    return this->errors;
}


int Parser::goTo(int nonterm, int term) {
    if(nonterm >= transitions.size() || term >= transitions.at(nonterm).size() + 1) {
        return -1;
    } else if(term == EOP) {
        return transitions.at(nonterm).at(EOP);
    }

    return transitions.at(nonterm).at(term);
}

stl_tree::tree<st_data> Parser::parse() {
    symbol start = rules[0].left;
    stl_tree::tree<st_data> tr(start.tag);

    if(!tokens.hasNext())
        return tr;

    Token tok = tokens.next();
    st_iter parent = tr.begin();

    // put EOP on top of stack
    this->symbols.push(symbol(TERM, EOP));
    // put start non-term (left non-term of first rule)
    this->symbols.push(start);

    this->nodes.push(parent);

    do {
        symbol top = this->symbols.top();

        if(top.type == TERM) {
            if (top.tag == tok.tag) {
                std::cout << "shift " << tokens.tag_to_tok.at(tok.tag) << std::endl;
                if(!nodes.empty()) {
                    this->nodes.top()->data = tok;
                    this->nodes.pop();
                }
                this->symbols.pop();


                if (tokens.hasNext()) {
                    tok = tokens.next();
                } else {
                    break;
                }
            } else if(top.tag == EMPTY) {
                Token emptyTok = Token(tok);
                emptyTok.frag.follow = emptyTok.frag.begin;
                emptyTok.tag = EMPTY;
                this->nodes.top()->data = emptyTok;

                std::cout << "empty" << std::endl;
                this->symbols.pop();
                this->nodes.pop();
            } else {
                errors.push_back(invalidToken(tok));
                if(tokens.hasNext()) {
                    tok = tokens.next();
                } else {
                    break;
                }
            }
        } else {
            int ruleIdx;
            if((ruleIdx = goTo(top.tag, tok.tag)) >= 0) {
                std::cout << "reduce " << ruleIdx << std::endl;

                std::vector<symbol> &rs = rules[ruleIdx].right;
                parent = nodes.top();

                symbols.pop();
                nodes.pop();


                for (int i = (int)rs.size() - 1; i >= 0; --i) {
                    symbols.push(rs[i]);
                    nodes.push(tr.append_child(parent, rs[i].tag));
//                    if(rs[i].type == NONTERM) {
//                        nodes.push(tr.append_child(parent, rs[i].tag));
//                    }
                }
            } else {
                if(tok.tag == EOP) {
                    errors.emplace_back("Syntax error: unexpected end of program");
                } else {
                    errors.emplace_back(invalidToken(tok));
                    std::cout << invalidToken(tok) << std::endl;
                }
                if(tokens.hasNext())
                    tok = tokens.next();
                else {
                    break;
                }
            }
        }
        std::cout << std::endl;
    } while(!symbols.empty());

    while(!symbols.empty()) {
        int ruleIdx;
        symbol top = symbols.top();
        if(top.type == NONTERM && (ruleIdx = goTo(top.tag, tok.tag)) >= 0) {
            // ???
        } else if(top.type == TERM) {
            errors.emplace_back("Syntax error: unexpected term - " + std::to_string(top.tag) );
        }
        symbols.pop();
        nodes.pop();
    }

    return tr;
}

std::string Parser::invalidToken(Token tok) {
    return "Syntax error at " + tok.frag.begin.str() + ": unexpected token - \"" + tokens.tag_to_tok.at(tok.tag) + "\"";
}

void Parser::printStack() {
    std::stack<symbol> stack = symbols;
    while(!stack.empty()) {
        std::cout << (stack.top().type == TERM ? "T-" : "NT-") << stack.top().tag << " ";
        stack.pop();
    }

    std::cout << std::endl;
}
