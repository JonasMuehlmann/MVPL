#ifndef SRC_TYPES_TOKEN_HPP
#define SRC_TYPES_TOKEN_HPP
enum class token
{
    // Syntax
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    LBRACAE,
    RBRACE,
    SEMICOLON,
    COMMA,
    IDENTIFIER,
    LITERAL,
    ASSIGN,
    // Operators
    LESS,
    LESSEQ,
    GREATER,
    GREATEREQ,
    EQUAL,
    NEQUAL,
    NOT,
    LOGICAL_AND,
    LOGICAL_OR,
    BINARY_AND,
    BINARY_OR,
    LSHIFT,
    RSHIFT,
    XOR,
    PLUS,
    MINUS,
    MULTIPLICATION,
    DIVISION,
    MODULO,
    INCREMENT,
    DECREMENT,
    // Flow control
    WHILE,
    FOR,
    IF,
    ELIF,
    ELSE,
    // Misc
    FUNCTION,
    PROCEDURE,
    RETURN
};
#endif    // SRC_TYPES_TOKEN_HPP
