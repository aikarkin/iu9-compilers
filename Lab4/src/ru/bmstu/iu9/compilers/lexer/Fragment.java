package ru.bmstu.iu9.compilers.lexer;

public class Fragment {
    private Position begin, follow;

    public Fragment(Position begin, Position follow) {
        this.begin = begin;
        this.follow = follow;
    }

    public Position begining() {
        return begin;
    }

    public Position following() {
        return follow;
    }

    @Override
    public String toString() {
        return begin + "-" + follow;
    }
}
