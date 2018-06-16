//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_POSITION_H
#define LAB7_1_POSITION_H

#endif //LAB7_1_POSITION_H

#include<sstream>

struct Position {
    unsigned long offset;
    unsigned long line;
    unsigned long pos;
    std::string src;

    std::string str() {
        std::stringstream ss;
        ss << '(' << line << ',' << pos << ')';
        return ss.str();
    }

    Position() {
        this->offset = 0;
        this->line = 1;
        this->pos = 1;
    }

    Position(std::string &src) {
        this->offset = 0;
        this->line = 1;
        this->pos = 1;
        this->src = src;
    }

    Position(const Position &pos) {
        this->offset = pos.offset;
        this->line = pos.line;
        this->pos = pos.pos;
        this->src = pos.src;
    }

    Position inc() {
        Position pos = *this;


        if(pos.offset < pos.src.length()) {
            char ch = pos.src.at(pos.offset);

            if (ch == '\n') {
                pos.line++;
                pos.pos = 1;
            } else {
                pos.line = this->line;
                pos.pos = this->pos + 1;
            }
        } else {
            pos.pos = this->pos + 1;
        }

        pos.offset = this->offset + 1;

        return pos;
    }
};