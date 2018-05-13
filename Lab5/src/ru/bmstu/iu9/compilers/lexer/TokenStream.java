package ru.bmstu.iu9.compilers.lexer;

import ru.bmstu.iu9.compilers.machine.State;
import ru.bmstu.iu9.compilers.machine.StateMachine;

public class TokenStream {
    private final static StateMachine machine = new StateMachine();
    private String src;
    private MessageList messages;
    private Position pos;

    public TokenStream(String source) {
        this.src = source;
        this.messages = new MessageList();
        this.pos = new Position(src);
    }

    public boolean hasNext() {
        return this.pos.getIndex() < src.length();
    }

    public Token next() {
        if(!pos.hasNext())
            return null;

//        System.out.println("Parsing token ...");
        State cur = State.START, next = cur.clone();
        Position begin = pos.clone();
        Position following = begin.inc();

        while(pos.getIndex() <= pos.src.length() && next != null) {
//            System.out.printf("Current state: (%d, '%s'); hasNext: %b\n", cur.getIndex(), pos.ch(), pos.hasNext());
            cur = next.clone();
            next = machine.goToState(cur, pos.ch());
            following = pos.clone();
            pos = pos.inc();
        }

        Token tok;

        if(cur.isFinal()) {
            if(following.equals(begin))
                following = following.inc();

            tok = new Token(new Fragment(begin, following), cur.getDomainTag());
        } else {
            messages.addError(begin, "Invalid token '" + begin.ch() + "'");
            following = following.inc();
            pos = following;
            tok = new Token(new Fragment(begin, following), DomainTag.ERROR);
        }

        if(pos.hasNext())
            pos = following.clone();


        return  tok;
    }

    public MessageList getMessages() {
        return this.messages;
    }
}
