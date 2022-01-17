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
//                               program_parser                               //
//****************************************************************************//
TEST(TestProgramParser, 2VarDecls)
{
    std::array token_stream_raw{
                                token(token_type::LET, "let"sv, source_location()),
                                token(token_type::IDENTIFIER, "x"sv, source_location()),
                                token(token_type::SEMICOLON, ";"sv, source_location()) ,
                                token(token_type::LET, "let"sv, source_location()),
                                token(token_type::IDENTIFIER, "y"sv, source_location()),
                                token(token_type::SEMICOLON, ";"sv, source_location()) 
    };

    std::span<token> token_stream(token_stream_raw);

    auto result = program_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);

    auto globals = std::move(std::get<program_node>((*get_node(result))).globals);
    ASSERT_EQ(globals.size(), 2);
    ASSERT_TRUE(std::holds_alternative<var_decl_node>(*(globals[0])));
    ASSERT_TRUE(std::holds_alternative<var_decl_node>(*(globals[1])));
}
//****************************************************************************//
//                               ParameterDefParser                           //
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

//****************************************************************************//
//                              binary_op_parser                              //
//****************************************************************************//
TEST(TestBinaryOpParser, Complex)
{
    std::array token_stream_raw{
        token(token_type::PLUS, "+"sv, source_location()),
        token(token_type::LITERAL, "5"sv, source_location()),
        token(token_type::MULTIPLICATION, "*"sv, source_location()),
        token(token_type::LITERAL, "2"sv, source_location()),
        token(token_type::MULTIPLICATION, "/"sv, source_location()),
        token(token_type::LITERAL, "3"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto lhs_ = token_parser<token_type::LITERAL>::parse(token_stream.subspan(1, 1));

    auto result = binary_op_parser::parse(token_stream, lhs_);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);

    ASSERT_TRUE(std::holds_alternative<binary_op_node>((*get_node(result))));
    auto bin_op = std::move(std::get<binary_op_node>((*get_node(result))));

    // 5
    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(bin_op.lhs)));
    auto lhs = std::get<leaf_node>(*(bin_op.lhs));
    ASSERT_EQ(lhs.token, token_type::LITERAL);

    // 5 * 2 / 3
    ASSERT_TRUE(std::holds_alternative<binary_op_node>(*(bin_op.rhs)));
    auto rhs = std::move(std::get<binary_op_node>(*(bin_op.rhs)));

    // 5
    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(rhs.lhs)));
    auto rlhs = std::move(std::get<leaf_node>(*(rhs.lhs)));
    ASSERT_EQ(rlhs.token, token_type::LITERAL);

    // 2 / 3
    ASSERT_TRUE(std::holds_alternative<binary_op_node>(*(rhs.rhs)));
    auto rrhs = std::move(std::get<binary_op_node>(*(rhs.rhs)));

    // 2
    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(rrhs.lhs)));
    auto rrlhs = std::move(std::get<leaf_node>(*(rrhs.lhs)));
    ASSERT_EQ(rrlhs.token, token_type::LITERAL);

    // 3
    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(rrhs.rhs)));
    auto rrrhs = std::move(std::get<leaf_node>(*(rrhs.rhs)));
    ASSERT_EQ(rrrhs.token, token_type::LITERAL);
}

//****************************************************************************//
//                               unar_op_parser                               //
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
    auto unary_op = std::move(std::get<unary_op_node>((*get_node(result))));

    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(unary_op.operand)));
    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(unary_op.operator_)));

    auto operand = std::get<leaf_node>(*(unary_op.operand));
    ASSERT_EQ(operand.token, token_type::IDENTIFIER);

    auto operator_ = std::get<leaf_node>(*(unary_op.operator_));
    ASSERT_EQ(operator_.token, token_type::NOT);
}
