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
//
//    this->terminal_characters.emplace('@'); // 64
//    this->terminal_characters.emplace('['); // 91
//    this->terminal_characters.emplace(']'); // 93
//    this->terminal_characters.emplace('('); // 40
//    this->terminal_characters.emplace(')'); // 41
//    this->terminal_characters.emplace('{'); // 123
//    this->terminal_characters.emplace('}'); // 125
//    this->terminal_characters.emplace(':'); // 58
//    this->terminal_characters.emplace(','); // 44


    this->tok_to_tag.emplace("$", 0);
    this->tok_to_tag.emplace("{", 1);
    this->tok_to_tag.emplace("NONTERM", 2);
    this->tok_to_tag.emplace("}", 3);
    this->tok_to_tag.emplace(",", 4);
    this->tok_to_tag.emplace("[", 5);
    this->tok_to_tag.emplace(":", 6);
    this->tok_to_tag.emplace("]", 7);
    this->tok_to_tag.emplace("TERM", 8);
    this->tok_to_tag.emplace("@", 9);

    for(auto const & [tok, tag] : tok_to_tag) {
        tag_to_tok.emplace(tag, tok);
    }
}

Token TokenStream::next() {
    Token tok;
    this->pos = skipSpaces();

    // END OF PROGRAM
    if(this->pos.offset >= this->source.length()) {
        tok.frag.begin = this->pos;
        this->pos = this->pos.inc();
        tok.frag.follow = this->pos;
        tok.tag = tok_to_tag.at("$");

    } else {
        tok.frag.begin = this->pos;

        char ch = this->source.at(this->pos.offset);

        const char term[] = {ch};

        // single char term
        if(tok_to_tag.count(term)) {
            this->pos = this->pos.inc();
            tok.frag.follow = this->pos;

            tok.tag = tok_to_tag.at(term);
//            std::cout << "recognize " << ch << std::endl;
        // quoted term (TERM)
        } else if(ch == '\"') {
            std::string term = this->parseString();
            tok.frag.follow = this->pos;
            tok.tag = tok_to_tag.at("TERM");
            tok.value = term;

        // NON TERM
        } else if(std::isupper(ch)) {
            std::string nonTerm = this->parseNonterm();
            tok.frag.follow = this->pos;
            tok.tag = tok_to_tag.at("NONTERM");
            tok.value = nonTerm;
        // INVALID TOKEN
        } else {
            invalidToken(pos, ch);
            this->pos = this->pos.inc();
        }
    }

    return tok;
}

bool TokenStream::hasNext() {
    return this->pos.offset <= this->source.length();
}

void TokenStream::invalidToken(Position pos, char ch) {
    std::stringstream ss;
    ss << "Lexical error: unknown token '" << ch << "' at " << pos.str() << std::endl;
    
    
    std::string error = ss.str();
    this->errors->push_back(error);
    
}

Position TokenStream::skipSpaces() {
    Position pos = this->pos;
    if(this->pos.offset >= this->source.length())
        return pos;

    char ch = this->source.at(pos.offset);

    while(ch == ' ' || ch == '\t' || ch == '\n') {
        pos = pos.inc();
        if(pos.offset >= source.length())
            return pos;
        ch = this->source.at(pos.offset);
    }

    return pos;
}

std::vector<std::string> &TokenStream::getErrors() {
    return *this->errors;
}

std::string TokenStream::parseString() {
    std::string str = "\"";
    this->pos = this->pos.inc();
    char ch;

    while(this->pos.offset < this->source.length() && (ch = this->source.at(this->pos.offset)) != '\"') {
        str += ch;
        this->pos = this->pos.inc();
    }

    str += ch;

    if(this->pos.offset + 1 < this->source.length()) {
        this->pos = this->pos.inc();
    }

    return str;
}

std::string TokenStream::parseNonterm() {
    if(this->pos.offset >= this->source.length())
        return "";

    std::string str = "";
    char ch;

    while(this->pos.offset < this->source.length()) {
        ch = this->source.at(this->pos.offset);
        if (ch != '\'' && !std::isalnum(ch)) {
            break;
        }

        str += ch;
        this->pos = this->pos.inc();
    }

//    std::cout << std::endl;

    return str;
}

void TokenStream::printDomainTags() {
    std::cout << "{" << std::endl;
    for (auto const& tok_tag : tok_to_tag) {
        std::cout << "\t'" << tok_tag.first << "': " << tok_tag.second << ", " << std::endl;
    }
    std::cout << "}" << std::endl;
}

void TokenStream::reset() {
    this->pos = Position();
}