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

    public String getTokenRepresentation() {
        String src = begin.src;
        return src
                .substring(begin.getIndex(), follow.getIndex())
                .replaceAll("\n", "\\\\n")
                .replaceAll("\t", "\\\\t");
    }

    @Override
    public String toString() {
        return begin.toString() + "-" + follow.toString();
    }
}
