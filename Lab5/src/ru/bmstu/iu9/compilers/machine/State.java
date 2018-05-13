package ru.bmstu.iu9.compilers.machine;

import ru.bmstu.iu9.compilers.lexer.DomainTag;

import java.util.HashMap;
import java.util.Map;

public class State {
    public static final Map<Integer, DomainTag> FINAL_STATES = new HashMap<>();
    public static final State START = new State(0);

    static {
        for(DomainTag tag: DomainTag.values()) {
            for(int state : tag.getStates()) {
                FINAL_STATES.put(state, tag);
            }
        }
    }

    private int idx;

    public State(int i) {
        this.idx = i;
    }

    public boolean isFinal() {
        return FINAL_STATES.containsKey(this.idx);
    }

    public int getIndex() {
        return this.idx;
    }

    public DomainTag getDomainTag() {
        return this.isFinal() ? FINAL_STATES.get(this.idx) : null;
    }

    public State clone() {
        return new State(this.idx);
    }
}
