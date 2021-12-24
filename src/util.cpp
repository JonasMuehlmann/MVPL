#include "util.hpp"

#include <iostream>
#include <stdexcept>

#include "lexeme.hpp"

std::string token_to_str(const token t)
{
    switch (t)
    {
        case token::LPAREN:
            return "LPAREN";
        case token::RPAREN:
            return "RPAREN";
        case token::LBRACKET:
            return "LBRACKET";
        case token::RBRACKET:
            return "RBRACKET";
        case token::LBRACAE:
            return "LBRACAE";
        case token::RBRACE:
            return "RBRACE";
        case token::SEMICOLON:
            return "SEMICOLON";
        case token::COMMA:
            return "COMMA";
        case token::IDENTIFIER:
            return "IDENTIFIER";
        case token::LESS:
            return "LESS";
        case token::GREATER:
            return "GREATER";
        case token::EQUAL:
            return "EQUAL";
        case token::NOT:
            return "NOT";
        case token::LOGICAL_AND:
            return "LOGICAL_AND";
        case token::LOGICAL_OR:
            return "LOGICAL_OR";
        case token::BINARY_AND:
            return "BINARY_AND";
        case token::BINARY_OR:
            return "BINARY_OR";
        case token::LSHIFT:
            return "LSHIFT";
        case token::RSHIFT:
            return "RSHIFT";
        case token::XOR:
            return "XOR";
        case token::PLUS:
            return "PLUS";
        case token::MINUS:
            return "MINUS";
        case token::MULTIPLICATION:
            return "MULTIPLICATION";
        case token::DIVISION:
            return "DIVISION";
        case token::MODULO:
            return "MODULO";
        case token::INCREMENT:
            return "INCREMENT";
        case token::DECREMENT:
            return "DECREMENT";
        case token::WHILE:
            return "WHILE";
        case token::FOR:
            return "FOR";
        case token::IF:
            return "IF";
        case token::ELIF:
            return "ELIF";
        case token::ELSE:
            return "";
        case token::FUNCTION:
            return "FUNCTION";
        case token::PROCEDURE:
            return "PROCEDURE";
        case token::RETURN:
            return "RETURN";
        default:
            throw std::invalid_argument("Unrecognized token");
    }
}
token str_to_token(const std::string& str)
{
    if (str == "LPAREN")
        return token::LPAREN;
    if (str == "RPAREN")
        return token::RPAREN;
    if (str == "LBRACKET")
        return token::LBRACKET;
    if (str == "RBRACKET")
        return token::RBRACKET;
    if (str == "LBRACAE")
        return token::LBRACAE;
    if (str == "RBRACE")
        return token::RBRACE;
    if (str == "SEMICOLON")
        return token::SEMICOLON;
    if (str == "COMMA")
        return token::COMMA;
    if (str == "IDENTIFIER")
        return token::IDENTIFIER;
    if (str == "LESS")
        return token::LESS;
    if (str == "GREATER")
        return token::GREATER;
    if (str == "EQUAL")
        return token::EQUAL;
    if (str == "NOT")
        return token::NOT;
    if (str == "LOGICAL_AND")
        return token::LOGICAL_AND;
    if (str == "LOGICAL_OR")
        return token::LOGICAL_OR;
    if (str == "BINARY_AND")
        return token::BINARY_AND;
    if (str == "BINARY_OR")
        return token::BINARY_OR;
    if (str == "LSHIFT")
        return token::LSHIFT;
    if (str == "RSHIFT")
        return token::RSHIFT;
    if (str == "XOR")
        return token::XOR;
    if (str == "PLUS")
        return token::PLUS;
    if (str == "MINUS")
        return token::MINUS;
    if (str == "MULTIPLICATION")
        return token::MULTIPLICATION;
    if (str == "DIVISION")
        return token::DIVISION;
    if (str == "MODULO")
        return token::MODULO;
    if (str == "INCREMENT")
        return token::INCREMENT;
    if (str == "DECREMENT")
        return token::DECREMENT;
    if (str == "WHILE")
        return token::WHILE;
    if (str == "FOR")
        return token::FOR;
    if (str == "IF")
        return token::IF;
    if (str == "ELIF")
        return token::ELIF;
    if (str == "ELSE")
        return token::ELSE;
    if (str == "FUNCTION")
        return token::FUNCTION;
    if (str == "PROCEDURE")
        return token::PROCEDURE;
    if (str == "RETURN")
        return token::RETURN;
    throw std::invalid_argument("Unrecognized token");
}

void print_token_stream(const std::vector<lexeme>& token_stream)
{
    for (auto lexeme_ : token_stream)
    {
        std::cout << "Token: token=" << token_to_str(lexeme_.token)
                  << ", value=" << lexeme_.value << '\n';
    }
}
