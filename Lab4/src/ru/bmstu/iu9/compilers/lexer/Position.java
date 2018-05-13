package ru.bmstu.iu9.compilers.lexer;

public class Position {
    private int index, line, column;
    private String src;

    public Position(String source) {
        this.src = source;
        this.index = 0;
        this.line = 1;
        this.column = 1;
    }


    public Position(Position pos) {
        this.src = pos.src;
        this.index = pos.index;
        this.column = pos.column;
        this.line = pos.line;
    }

    public Position inc() {
        Position pos = new Position(this);

        if(src.charAt(pos.index) == '\n') {
            pos.line++;
            pos.column = 1;
        } else {
            pos.column++;
        }

        pos.index++;

        return pos;
    }

    public char ch() { return index >= src.length() ? src.charAt(src.length() - 1) : src.charAt(index) ; }

    public int getIndex() {
        return index;
    }

    public int getLine() {
        return line;
    }

    public int getColumn() {
        return column;
    }

    @Override
    public String toString() {
        return String.format("(%d, %d)", getLine(), getColumn());
    }
}
