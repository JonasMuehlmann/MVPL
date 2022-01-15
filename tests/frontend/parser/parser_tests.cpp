#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <span>
#include <variant>
#include <vector>

#include "../src/frontend/lexer/token.hpp"
#include "../src/frontend/lexer/token_type.hpp"
#include "../src/frontend/parser/parser.hpp"

using namespace std::string_view_literals;


//****************************************************************************//
//                               ParameterDefParser                              //
//****************************************************************************//
TEST(TestParameterDefParser, NoParameter)
{
    std::array token_stream_raw{token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_def_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_EQ(std::get<parameter_def_node>((*get_node(result))).parameter_list.size(),
              0);
}

TEST(TestParameterDefParser, OneParameter)
{
    std::array token_stream_raw{
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_def_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<parameter_def_node>((*get_node(result))));
    ASSERT_EQ(std::get<parameter_def_node>((*get_node(result))).parameter_list.size(),
              1);
    ASSERT_EQ(std::get<parameter_def_node>((*get_node(result))).parameter_list[0],
              "foo"sv);
}

TEST(TestParameterDefParser, TwoParameters)
{
    std::array token_stream_raw{
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::IDENTIFIER, "bar"sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_def_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<parameter_def_node>((*get_node(result))));
    ASSERT_EQ(std::get<parameter_def_node>((*get_node(result))).parameter_list.size(),
              2);
    ASSERT_TRUE(std::ranges::equal(
        std::get<parameter_def_node>((*get_node(result))).parameter_list,
        std::array{"foo"sv, "bar"sv}));
}

TEST(TestParameterDefParser, ThreeParameters)
{
    std::array token_stream_raw{
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::IDENTIFIER, "bar"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::IDENTIFIER, "baz"sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_def_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<parameter_def_node>((*get_node(result))));
    ASSERT_EQ(std::get<parameter_def_node>((*get_node(result))).parameter_list.size(),
              3);
    ASSERT_TRUE(std::ranges::equal(
        std::get<parameter_def_node>((*get_node(result))).parameter_list,
        std::array{"foo"sv, "bar"sv, "baz"sv}));
}

TEST(TestParameterDefParser, FourParameters)
{
    std::array token_stream_raw{
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::IDENTIFIER, "bar"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::IDENTIFIER, "baz"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::IDENTIFIER, "test"sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_def_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<parameter_def_node>((*get_node(result))));
    ASSERT_EQ(std::get<parameter_def_node>((*get_node(result))).parameter_list.size(),
              4);
    ASSERT_TRUE(std::ranges::equal(
        std::get<parameter_def_node>((*get_node(result))).parameter_list,
        std::array{"foo"sv, "bar"sv, "baz"sv, "test"sv}));
}

TEST(TestParameterDefParser, TrailingComma)
{
    std::array token_stream_raw{
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_def_parser::parse(token_stream);

    ASSERT_FALSE(std::holds_alternative<parse_content>(result));
}

//****************************************************************************//
//                               ParameterPassParser                          //
//****************************************************************************//
TEST(TestParameterPassParser, NoParameter)
{
    std::array token_stream_raw{token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_pass_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_EQ(std::get<parameter_pass_node>((*get_node(result))).parameter_list.size(),
              0);
}

TEST(TestParameterPassParser, OneParameter)
{
    std::array token_stream_raw{
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_pass_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<parameter_pass_node>((*get_node(result))));
    ASSERT_EQ(std::get<parameter_pass_node>((*get_node(result))).parameter_list.size(),
              1);
    ASSERT_EQ(std::get<parameter_pass_node>((*get_node(result))).parameter_list[0],
              "foo"sv);
}

TEST(TestParameterPassParser, TwoParameters)
{
    std::array token_stream_raw{
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::IDENTIFIER, "bar"sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_pass_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<parameter_pass_node>((*get_node(result))));
    ASSERT_EQ(std::get<parameter_pass_node>((*get_node(result))).parameter_list.size(),
              2);
    ASSERT_TRUE(std::ranges::equal(
        std::get<parameter_pass_node>((*get_node(result))).parameter_list,
        std::array{"foo"sv, "bar"sv}));
}

TEST(TestParameterPassParser, ThreeParameters)
{
    std::array token_stream_raw{
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::IDENTIFIER, "bar"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::IDENTIFIER, "baz"sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_pass_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<parameter_pass_node>((*get_node(result))));
    ASSERT_EQ(std::get<parameter_pass_node>((*get_node(result))).parameter_list.size(),
              3);
    ASSERT_TRUE(std::ranges::equal(
        std::get<parameter_pass_node>((*get_node(result))).parameter_list,
        std::array{"foo"sv, "bar"sv, "baz"sv}));
}

TEST(TestParameterPassParser, FourParameters)
{
    std::array token_stream_raw{
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::IDENTIFIER, "bar"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::IDENTIFIER, "baz"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::IDENTIFIER, "test"sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_pass_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<parameter_pass_node>((*get_node(result))));
    ASSERT_EQ(std::get<parameter_pass_node>((*get_node(result))).parameter_list.size(),
              4);
    ASSERT_TRUE(std::ranges::equal(
        std::get<parameter_pass_node>((*get_node(result))).parameter_list,
        std::array{"foo"sv, "bar"sv, "baz"sv, "test"sv}));
}

TEST(TestParameterPassParser, TrailingComma)
{
    std::array token_stream_raw{
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::COMMA, ","sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_pass_parser::parse(token_stream);

    ASSERT_FALSE(std::holds_alternative<parse_content>(result));
}

//****************************************************************************//
//                               var_init_parser                              //
//****************************************************************************//
TEST(TestVarInitParser, Literal)
{
    std::array token_stream_raw{token(token_type::LET, "let"sv, source_location()),
                                token(token_type::IDENTIFIER, "x"sv, source_location()),
                                token(token_type::ASSIGN, "="sv, source_location()),
                                token(token_type::LITERAL, "5"sv, source_location()),
                                token(token_type::SEMICOLON, ";"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = var_init_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<var_init_node>((*get_node(result))));
    ASSERT_EQ(std::get<var_init_node>((*get_node(result))).identifier, "x"sv);
    ASSERT_NE(std::get<var_init_node>((*get_node(result))).value, nullptr);

    ASSERT_TRUE((std::holds_alternative<leaf_node>(
        *(std::get<var_init_node>((*get_node(result))).value))));

    ASSERT_EQ(
        (std::get<leaf_node>(*(std::get<var_init_node>((*get_node(result))).value)))
            .token,
        token_type::LITERAL);
}

TEST(TestVarInitParser, Identifier)
{
    std::array token_stream_raw{token(token_type::LET, "let"sv, source_location()),
                                token(token_type::IDENTIFIER, "x"sv, source_location()),
                                token(token_type::ASSIGN, "="sv, source_location()),
                                token(token_type::IDENTIFIER, "y"sv, source_location()),
                                token(token_type::SEMICOLON, ";"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = var_init_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<var_init_node>((*get_node(result))));
    ASSERT_EQ(std::get<var_init_node>((*get_node(result))).identifier, "x"sv);
    ASSERT_NE(std::get<var_init_node>((*get_node(result))).value, nullptr);

    ASSERT_TRUE((std::holds_alternative<leaf_node>(
        *(std::get<var_init_node>((*get_node(result))).value))));

    ASSERT_EQ(
        (std::get<leaf_node>(*(std::get<var_init_node>((*get_node(result))).value)))
            .token,
        token_type::IDENTIFIER);

    ASSERT_EQ(
        (std::get<leaf_node>(*(std::get<var_init_node>((*get_node(result))).value)))
            .value,
        "y"sv);
}

//****************************************************************************//
//                              expression_parser                             //
//****************************************************************************//
TEST(TestExpressionParser, Identifier)
{
    std::array token_stream_raw{
        token(token_type::IDENTIFIER, "x"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = expression_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<leaf_node>((*get_node(result))));
    ASSERT_EQ(std::get<leaf_node>((*get_node(result))).token, token_type::IDENTIFIER);
    ASSERT_EQ(std::get<leaf_node>((*get_node(result))).value, "x"sv);
}

TEST(TestExpressionParser, Literal)
{
    std::array token_stream_raw{token(token_type::LITERAL, "5"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = expression_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<leaf_node>((*get_node(result))));
    ASSERT_EQ(std::get<leaf_node>((*get_node(result))).token, token_type::LITERAL);
    ASSERT_EQ(std::get<leaf_node>((*get_node(result))).value, "5"sv);
}

TEST(TestExpressionParser, BinaryOp)
{
    std::array token_stream_raw{token(token_type::LITERAL, "5"sv, source_location()),
                                token(token_type::PLUS, "+"sv, source_location()),
                                token(token_type::LITERAL, "5"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = expression_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<binary_op_node>((*get_node(result))));
    ASSERT_EQ(
        std::get<leaf_node>(*(std::get<binary_op_node>((*get_node(result))).lhs)).token,
        token_type::LITERAL);
    ASSERT_EQ(
        std::get<leaf_node>(*(std::get<binary_op_node>((*get_node(result))).operator_))
            .token,
        token_type::PLUS);
    ASSERT_EQ(
        std::get<leaf_node>(*(std::get<binary_op_node>((*get_node(result))).rhs)).token,
        token_type::LITERAL);
}

TEST(TestExpressionParser, ComplexBinaryOps)
{
    std::array token_stream_raw{
        token(token_type::LITERAL, "5"sv, source_location()),
        token(token_type::PLUS, "+"sv, source_location()),
        token(token_type::LITERAL, "5"sv, source_location()),
        token(token_type::MULTIPLICATION, "*"sv, source_location()),
        token(token_type::LITERAL, "2"sv, source_location()),
        token(token_type::MULTIPLICATION, "/"sv, source_location()),
        token(token_type::LITERAL, "3"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = expression_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<binary_op_node>((*get_node(result))));
    // 5
    ASSERT_EQ(
        std::get<leaf_node>(*(std::get<binary_op_node>((*get_node(result))).lhs)).token,
        token_type::LITERAL);
    // 5 * 2 / 3
    ASSERT_EQ(
        std::get<leaf_node>(*std::get<binary_op_node>(
                                 *(std::get<binary_op_node>((*get_node(result))).rhs))
                                 .lhs)
            .token,
        token_type::LITERAL);
    // 2
    ASSERT_EQ(std::get<leaf_node>(
                  *std::get<binary_op_node>(
                       *std::get<binary_op_node>(
                            *(std::get<binary_op_node>((*get_node(result))).rhs))
                            .rhs)
                       .lhs)
                  .token,
              token_type::LITERAL);
    // 3
    ASSERT_EQ(std::get<leaf_node>(
                  *std::get<binary_op_node>(
                       *std::get<binary_op_node>(
                            *(std::get<binary_op_node>((*get_node(result))).rhs))
                            .rhs)
                       .rhs)
                  .token,
              token_type::LITERAL);
}
//****************************************************************************//
//                              expression_parser                             //
//****************************************************************************//
TEST(TestUnaryOpParser, NotIdentifier)
{
    std::array token_stream_raw{
        token(token_type::NOT, "!"sv, source_location()),
        token(token_type::IDENTIFIER, "x"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = unary_op_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<unary_op_node>((*get_node(result))));
    ASSERT_EQ(std::get<leaf_node>(*std::get<unary_op_node>((*get_node(result))).operand)
                  .token,
              token_type::IDENTIFIER);
    ASSERT_EQ(
        std::get<leaf_node>(*std::get<unary_op_node>((*get_node(result))).operator_)
            .token,
        token_type::NOT);
}
