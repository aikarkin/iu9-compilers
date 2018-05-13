package com.bmstu.iu9.compilers;

public class Token {
    private TokenType type;
    private String value;
    private Position pos;

    public Token(Position pos, TokenType type, String value) {
        this.pos = pos;
        this.type = type;
        this.value = value;
    }

    @Override
    public String toString() {
        return String.format("%s %s: %s", type.name(), pos.toString(), value);
    }

    public String getValue() {
        return value;
    }
}
