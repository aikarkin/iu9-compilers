package ru.bmstu.iu9.compilers.lexer;

public class Position {
    private int index, line, column;
    protected String src;

    public Position(String source) {
        this.src = source;
        this.index = 0;
        this.line = 1;
        this.column = 1;
    }

    public Position clone() {
        Position ret = new Position(src);
        ret.index = index;
        ret.line = line;
        ret.column = column;

        return ret;
    }

    public Position inc() {
        Position pos = this.clone();

        if(!hasNext()) {
            pos.index++;
            return pos;
        }

        if(src.charAt(pos.index) == '\n') {
            pos.line++;
            pos.column = 1;
        } else {
            pos.column++;
        }

        pos.index++;

        return pos;
    }

    public boolean hasNext() {
        return this.index < src.length();
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

    @Override
    public boolean equals(Object obj) {
        return obj instanceof Position && ((Position)obj).index == this.index;
    }
}
