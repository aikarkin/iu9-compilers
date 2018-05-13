package com.bmstu.iu9.compilers;

import javafx.geometry.Pos;

public class Position {
    private int offset, line, column;
    private String source;

    public Position(Position pos) {
        this.source = pos.source;
        this.offset = pos.offset;
        this.line = pos.line;
        this.column = pos.column;
    }

    public Position(String source) {
        this.source = source;
        this.offset = 0;
        this.line = 1;
        this.column = 1;
    }

    public Position increase(int d) {
        if(offset >= source.length()) {
            return this;
        }
        Position pos = new Position(source);
        int newLines = 0, lastNewLine = -1;

        for (int i = offset; i < offset + d; i++) {
            char ch = source.charAt(i);
            if(ch == '\n') {
                newLines++;
                lastNewLine = i;
            }
        }

        if(newLines > 0) {
            pos.line = this.line + newLines;
            pos.column = this.offset + d - lastNewLine;
        } else {
            pos.column = this.column + d;
            pos.line = this.line;
        }
        pos.offset = this.offset + d;

        return pos;
    }


    @Override
    public String toString() {
        return String.format("(%d, %d)", line, column);
    }

    public int getOffset() {
        return offset;
    }

    public String getSource() {
        return source;
    }
}
