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

    this->tag_id = 1;

    this->errors = std::make_shared< std::vector<std::string> >();

    this->terminal_characters.emplace('@');
    this->terminal_characters.emplace('[');
    this->terminal_characters.emplace(']');
    this->terminal_characters.emplace('(');
    this->terminal_characters.emplace(')');
    this->terminal_characters.emplace('{');
    this->terminal_characters.emplace('}');
    this->terminal_characters.emplace(':');
    this->terminal_characters.emplace(',');
    this->terminal_characters.emplace('+');
    this->terminal_characters.emplace('*');
}

Token TokenStream::next() {
    Token tok;
    this->pos = skipSpaces();

    // END OF PROGRAM
    if(this->pos.offset >= this->source.length()) {
        tok.frag.begin = this->pos;
        this->pos = this->pos.inc();
        tok.frag.follow = this->pos;

        if(tok_to_tag.count("$") == 0) {
            tag_to_tok.emplace(EOP, "$");
            tok_to_tag.emplace("$", EOP);
        }
        tok.tag = 0;
    } else {
        tok.frag.begin = this->pos;

        char ch = this->source.at(this->pos.offset);

        // single char term
        if(terminal_characters.count(ch) > 0) {
            this->pos = this->pos.inc();
            tok.frag.follow = this->pos;

            const char term[] = {ch};
            if(tok_to_tag.count(term) == 0) {
                tag_to_tok.emplace(tag_id, term);
                tok_to_tag.emplace(term, tag_id++);
            }
            tok.tag = tok_to_tag.at(term);

        // quoted term (TERM)
        } else if(ch == '\"') {
            std::string term = this->parseString();
            tok.frag.follow = this->pos;

            if(tok_to_tag.count("TERM") == 0) {
                tag_to_tok.emplace(tag_id, term);
                tok_to_tag.emplace("TERM", tag_id++);
            }

            tok.tag = tok_to_tag.at("TERM");
            tok.value = term;

        // NON TERM
        } else if(std::isupper(ch)) {
            std::string nonTerm = this->parseNonterm();
            tok.frag.follow = this->pos;
            if(tok_to_tag.count("NONTERM") == 0) {
                tag_to_tok.emplace(tag_id, nonTerm);
                tok_to_tag.emplace("NONTERM", tag_id++);
            }

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