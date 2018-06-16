//
// Created by alex on 03.05.18.
//

#include <iostream>
#include <utility>
#include "Parser.h"


std::vector<std::string> &gr_parser::Parser::getErrors() {
    return this->errors;
}


int gr_parser::Parser::goTo(int nonterm, int term) {
    if(nonterm >= transitions.size() || term >= transitions.at(nonterm).size() + 1) {
        return -1;
    } else if(term == gr_lexer::TAG_EOP) {
        return transitions.at(nonterm).at(gr_lexer::TAG_EOP);
    }

    return transitions.at(nonterm).at(term);
}

stl_tree::tree<gr_parser::st_data> gr_parser::Parser::parse(gr_lexer::TokenStream tokens) {
    symbol start = rules[0].left;
    stl_tree::tree<st_data> tr(start.tag);

    if(!tokens.hasNext())
        return tr;

    Token tok = tokens.next();
    st_iter parent = tr.begin();

    // put TAG_EOP on top of stack
    this->symbols.push(symbol(TERM, gr_lexer::TAG_EOP));
    // put start non-term (left non-term of first rule)
    this->symbols.push(start);

    this->nodes.push(parent);

    do {
        printStack();
        symbol top = this->symbols.top();

        if(top.type == TERM) {
            if (top.tag == tok.tag) {
                std::cout << "shift " << tok.str() << std::endl;
                if(!nodes.empty()) {
                    *(this->nodes.top()) = tok;
                    this->nodes.pop();
                }
                this->symbols.pop();


                if (tokens.hasNext()) {
                    tok = tokens.next();
                } else {
                    break;
                }
            } else if(top.tag == gr_lexer::TAG_EMPTY) {
                Token emptyTok = Token(tok);
                emptyTok.frag.follow = emptyTok.frag.begin;
                emptyTok.tag = gr_lexer::TAG_EMPTY;
                *(this->nodes.top()) = emptyTok;

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
                }
            } else {
                if(tok.tag == gr_lexer::TAG_EOP) {
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

std::string gr_parser::Parser::invalidToken(Token tok) {
    return "Syntax error at " + tok.frag.begin.str() + ": unexpected token - \"" + tok.str() + "\"";
}

void gr_parser::Parser::printStack() {
    std::stack<symbol> stack = symbols;
    while(!stack.empty()) {
        std::cout << (stack.top().type == TERM ? "T-" : "NT-") << stack.top().tag << " ";
        stack.pop();
    }

    std::cout << std::endl;
}

gr_parser::Parser::Parser(std::vector<std::vector<int>> transitions, std::vector<rule> rules)
    : errors(), transitions(std::move(transitions)), rules(std::move(rules))
{ }
