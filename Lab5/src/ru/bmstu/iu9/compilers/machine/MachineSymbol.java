package ru.bmstu.iu9.compilers.machine;

import java.util.regex.Pattern;

public enum MachineSymbol {
     /* symbol #0 */    WORD_SEP("\\s"),
     /* symbol #1 */    STRING_CHAR("[^\\n()]"),
     /* symbol #2 */    DIGIT("[0-9]"),
     /* symbol #3 */    LETTER("[a-zA-Z]"),
     /* symbol #4 */    LOWER_U("u"),
     /* symbol #5 */    LOWER_N("n"),
     /* symbol #6 */    LOWER_S("s"),
     /* symbol #7 */    LOWER_E("e"),
     /* symbol #8 */    LOWER_T("t"),
     /* symbol #9 */    NOT_LOWER_N("[a-zA-Z0-9&&[^n]]"),
     /* symbol #10 */   NOT_LOWER_S("[a-zA-Z0-9&&[^s]]"),
     /* symbol #11 */   NOT_LOWER_E("[a-zA-Z0-9&&[^e]]"),
     /* symbol #12 */   NOT_LOWER_T("[a-zA-Z0-9&&[^t]]"),
     /* symbol #13 */   NOT_LOWER_S_OR_U("[a-zA-Z0-9&&[^su]]"),
     /* symbol #14 */   LETER_OR_DIGIT("[a-zA-Z0-9]"),
     /* symbol #15 */   LBRACE("[(]"),
     /* symbol #16 */   RBRACE("[)]"),
    ;

    private Pattern ptr;

    MachineSymbol(String reg) {
        this.ptr = Pattern.compile(reg);
    }

    boolean matches(char ch) {
        String s = "" + ch;
        return this.ptr.matcher(s).lookingAt();
    }
}
