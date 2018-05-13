package ru.bmstu.iu9.compilers.lexer;


import ru.bmstu.iu9.compilers.tokens.BaseToken;
import ru.bmstu.iu9.compilers.tokens.Coefficient;
import ru.bmstu.iu9.compilers.tokens.Operator;
import ru.bmstu.iu9.compilers.tokens.Substance;
import java.util.Arrays;
import java.util.HashSet;


public class Scanner {
    private String src;
    private static final HashSet<Character> TOK_DELIMS = new HashSet<>(Arrays.asList(' ', '\n', '\r'));
    private MessageList msgs;
    private Position coord;

    private Position skipTokenDelimiters(Position pos) {
        Position follow = new Position(pos);
        while(follow.getIndex() < src.length() && TOK_DELIMS.contains(follow.ch())) {
            follow = follow.inc();
        }

        return follow;
    }

    public Scanner(String source) {
        this.src = source;
        this.msgs = new MessageList();
        this.coord = new Position(src);
    }

    public boolean hasNext() {
        return this.coord.getIndex() < src.length();
    }

    public BaseToken nextToken() {
        if(!hasNext())
            return null;

        BaseToken tok = null;

        Position begin = skipTokenDelimiters(coord);

        char ch = begin.ch();

        // coefficients
        if(Character.isDigit(ch)) {
            tok = parseCoefficient(begin);
        }
        // substances
        else if(Character.isAlphabetic(ch) && Character.isUpperCase(ch)) {
            tok = parseSubstance(begin);
        }
        // operators
        else if(ch == '+' || ch == '-') {
            tok = parseOperator(begin);
        }

        // error
        if(tok == null) {
            msgs.addError(begin, "Invalid token '" + ch + "'");
            coord = coord.inc();
        } else {
//            System.out.println("[dbg] cur tok: " + tok.toString());
            coord = tok.getFragment().following();
        }
        return tok;
    }

    private BaseToken parseOperator(Position begin) {
        Position follow;
        Operator.OperatorType type;

        if(begin.ch() == '+') {
            follow = begin.inc();
            type = Operator.OperatorType.PLUS;
        } else if(begin.ch() == '-') {
            if(begin.inc().getIndex() < src.length() && begin.inc().ch() == '>') {
                follow = begin.inc().inc();
                type = Operator.OperatorType.ARROW;
            } else {
                return null;
            }
        } else {
            return null;
        }

        return new Operator(new Fragment(begin, follow), type);
    }

    private BaseToken parseCoefficient(Position begin) {
        Position follow = new Position(begin);
        while (follow.getIndex() < src.length() && Character.isDigit(follow.ch())) {
            follow = follow.inc();
        }

        return new Coefficient(new Fragment(begin, follow), src.substring(begin.getIndex(), follow.getIndex()));
    }

    private BaseToken parseSubstance(Position begin) {
        Position prev = begin;
        Position follow = begin.inc();
        boolean isFirst = true;

        while(follow.getIndex() < src.length() && Character.isLetterOrDigit(follow.ch())) {
            if(isFirst) {
                isFirst = false;
            } else if(Character.isDigit(prev.ch()) && Character.isAlphabetic(follow.ch()) && Character.isLowerCase(follow.ch())) {
                    return null;
            }

            prev = follow;
            follow = follow.inc();
        }

        return new Substance(new Fragment(begin, follow), src.substring(begin.getIndex(), follow.getIndex()));
    }

    public String getSource() {
        return src;
    }

    public MessageList getMessages() {
        return msgs;
    }
}
