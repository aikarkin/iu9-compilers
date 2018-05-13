package ru.bmstu.iu9.compilers.machine;


public class StateMachine {
    private int[][] states = {
                         /* WS,  SCH,  DGT,  LET,    u,    n,    s,    e,    t,  NLn,  NLs,  NLe,  NLt, NLsu,  LOD,  LBR,   RBR  */
            /* state 0 */  { 8,   -1,    7,    -1,    2,   -1,    1,   -1,   -1,   -1,   -1,   -1,   -1,    6,    7,    9,   -1},
            /* state 1 */  {-1,   -1,   -1,   -1,   -1,   -1,   -1,    3,   -1,   -1,   -1,    6,   -1,   -1,   -1,   -1,   -1},
            /* state 2 */  {-1,   -1,   -1,   -1,   -1,    4,   -1,   -1,   -1,    6,   -1,   -1,   -1,   -1,   -1,   -1,   -1},
            /* state 3 */  {-1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    5,   -1,   -1,   -1,    6,   -1,   -1,   -1,   -1},
            /* state 4 */  {-1,   -1,   -1,   -1,   -1,   -1,    1,   -1,   -1,   -1,    6,   -1,   -1,   -1,   -1,   -1,   -1},
            /* state 5 */  {-1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    6,   -1,   -1},
            /* state 6 */  {-1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,    6,   -1,   -1},
            /* state 7 */  {-1,   -1,    7,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1},
            /* state 8 */  { 8,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1},
            /* state 9 */  {-1,    9,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   10},
            /* state 10 */ {-1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1},
    };


    public StateMachine() {}

    public State goToState(State state, char ch) {
        int idx = state.getIndex();
        for (int i = 0; i < states[idx].length; i++) {
            if(states[idx][i] != - 1 && MachineSymbol.values()[i].matches(ch)) {
                return new State(states[idx][i]);
            }
        }

        return null;
    }

}
