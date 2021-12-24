enum class opcode
{
    // Arithmetic
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    INC,
    DEC,
    // Binary
    AND,
    OR,
    NOT,
    LSHIFT,
    RSHIFT,
    XOR,

    // Comparison
    JUMPEQ,
    JUMPNEQ,
    JUMPLESS,
    JUMPGREATER,
    JUMPLEQ,
    JUMPGEQ,
    // Misc
    SET,
    SETLIT,
    JUMP,
    PRINT,
};
