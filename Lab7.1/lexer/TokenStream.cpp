//
// Created by alex on 03.05.18.
//

#include <sstream>
#include <iostream>
#include "TokenStream.h"

TokenStream::TokenStream(std::string &source) : pos(source) {
    this->source = source;
    this->pos.offset = 0;
    this->pos.line = 1;
    this->pos.pos = 1;
    this->errors = std::make_shared< std::vector<std::string> >();
}

Token TokenStream::next() {
    if(this->pos.offset >= this->source.length()) {
        this->pos = this->pos.inc();
        Fragment fragment{this->pos, this->pos.inc()};
        return Token(fragment, EOP);
    }

    DomainTag tag;
    this->pos = skipSpaces();
    Position start = this->pos;
    char ch = this->source.at(pos.offset);
    

    switch (ch) {
        case '+':
            tag = ADD;
            break;
        case '*':
            tag = MUL;
            break;
        case 'n':
            tag = NUM;
            break;
        case '(':
            tag = LPAREN;
            break;
        case ')':
            tag = RPAREN;
            break;
        default:
            
            invalidToken(pos, ch);
            tag = UNKNOWN;
            break;
    }

    this->pos = pos.inc();

    Fragment fragment{start, this->pos};
    Token tok(fragment, tag);

    return tok;
}

bool TokenStream::hasNext() {
    return this->pos.offset <= this->source.length();
}

void TokenStream::invalidToken(Position pos, char ch) {
    std::stringstream ss;
    ss << "Lexical error: unknown token '" << ch << "' at " << pos.toString() << std::endl;
    
    
    std::string error = ss.str();
    this->errors->push_back(error);
    
}

Position TokenStream::skipSpaces() {
    Position pos = this->pos;
    if(!hasNext())
        return pos;
    char ch = this->source.at(pos.offset);

    while(ch == ' ' || ch == '\t' || ch == '\n') {
        pos = pos.inc();
        ch = this->source.at(pos.offset);
    }

    return pos;
}

std::vector<std::string> &TokenStream::getErrors() {
    return *this->errors;
}

