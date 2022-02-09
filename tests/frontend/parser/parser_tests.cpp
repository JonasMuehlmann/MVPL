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
TEST(TestProgramParser, TwoVarDecls)
{
    std::array token_stream_raw{token(token_type::LET, "let"sv, source_location()),
                                token(token_type::IDENTIFIER, "x"sv, source_location()),
                                token(token_type::SEMICOLON, ";"sv, source_location()),
                                token(token_type::LET, "let"sv, source_location()),
                                token(token_type::IDENTIFIER, "y"sv, source_location()),
                                token(token_type::SEMICOLON, ";"sv, source_location())};

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
//                              binary_op_parser                              //
//****************************************************************************//
TEST(TestBinaryOpParser, Complex)
{
    std::array token_stream_raw{
        // NOTE: Imagine token(token_type::LITERAL, "5"sv, source_location())  here
        token(token_type::PLUS, "+"sv, source_location()),
        token(token_type::LITERAL, "5"sv, source_location()),
        token(token_type::MULTIPLICATION, "*"sv, source_location()),
        token(token_type::LITERAL, "2"sv, source_location()),
        token(token_type::DIVISION, "/"sv, source_location()),
        token(token_type::LITERAL, "3"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    // 5
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

    // (5 * 2) / 3
    ASSERT_TRUE(std::holds_alternative<binary_op_node>(*(bin_op.rhs)));
    auto rhs = std::move(std::get<binary_op_node>(*(bin_op.rhs)));

    // 5 * 2
    ASSERT_TRUE(std::holds_alternative<binary_op_node>(*(rhs.lhs)));
    auto rlhs = std::move(std::get<binary_op_node>(*(rhs.lhs)));

    // 3
    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(rhs.rhs)));
    auto rrhs = std::move(std::get<leaf_node>(*(rhs.rhs)));
    ASSERT_EQ(rrhs.token, token_type::LITERAL);

    // 5
    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(rlhs.lhs)));
    auto rllhs = std::move(std::get<leaf_node>(*(rlhs.lhs)));
    ASSERT_EQ(rllhs.token, token_type::LITERAL);

    // 2
    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(rlhs.rhs)));
    auto rlrhs = std::move(std::get<leaf_node>(*(rlhs.rhs)));
    ASSERT_EQ(rlrhs.token, token_type::LITERAL);
}

//****************************************************************************//
//                               unary_op_parser                              //
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
TEST(TestExpressionParser, BinaryOpParenthesis)
{
    std::array token_stream_raw{token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::LITERAL, "5"sv, source_location()),
                                token(token_type::PLUS, "+"sv, source_location()),
                                token(token_type::LITERAL, "5"sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location())};

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

TEST(TestExpressionParser, BinaryOpParenthesisComplex)
{
    std::array token_stream_raw{
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::LITERAL, "5"sv, source_location()),
        token(token_type::PLUS, "+"sv, source_location()),
        token(token_type::LITERAL, "5"sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location()),
        token(token_type::MULTIPLICATION, "*"sv, source_location()),
        token(token_type::LITERAL, "2"sv, source_location())};

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

TEST(TestExpressionParser, BinaryOpParenthesisNested)
{
    std::array token_stream_raw{token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::LITERAL, "5"sv, source_location()),
                                token(token_type::PLUS, "+"sv, source_location()),
                                token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::LITERAL, "1"sv, source_location()),
                                token(token_type::DIVISION, "/"sv, source_location()),
                                token(token_type::LITERAL, "2"sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location()),
                                token(token_type::PLUS, "*"sv, source_location()),
                                token(token_type::LITERAL, "3"sv, source_location())};

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
//                               func_def_parser                              //
//****************************************************************************//
TEST(TestFuncDefParser, NoParametersEmptyBody)
{
    std::array token_stream_raw{
        token(token_type::FUNCTION, "function"sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location()),
        token(token_type::LBRACE, "{"sv, source_location()),
        token(token_type::RBRACE, "}"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = func_def_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<func_def_node>(*get_node(result)));

    auto function_def = std::move(std::get<func_def_node>(*get_node(result)));

    ASSERT_TRUE(std::holds_alternative<signature_node>(*(function_def.signature)));
    ASSERT_TRUE(std::holds_alternative<block_node>(*(function_def.body)));

    auto signature = std::move(std::get<signature_node>(*(function_def.signature)));
    auto body      = std::move(std::get<block_node>(*(function_def.body)));

    auto parameter_list = std::get<parameter_def_node>(*(signature.parameter_list));
    ASSERT_TRUE(parameter_list.parameter_list.empty());

    ASSERT_TRUE(body.statements.empty());
}
//****************************************************************************//
//                            procedure_def_parser                            //
//****************************************************************************//
TEST(TestProcedureDefParser, NoParametersEmptyBody)
{
    std::array token_stream_raw{
        token(token_type::PROCEDURE, "procedure"sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location()),
        token(token_type::LBRACE, "{"sv, source_location()),
        token(token_type::RBRACE, "}"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = procedure_def_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<procedure_def_node>(*get_node(result)));

    auto procedure_def = std::move(std::get<procedure_def_node>(*get_node(result)));

    ASSERT_TRUE(std::holds_alternative<signature_node>(*(procedure_def.signature)));
    ASSERT_TRUE(std::holds_alternative<block_node>(*(procedure_def.body)));

    auto signature = std::move(std::get<signature_node>(*(procedure_def.signature)));
    auto body      = std::move(std::get<block_node>(*(procedure_def.body)));

    auto parameter_list = std::get<parameter_def_node>(*(signature.parameter_list));
    ASSERT_TRUE(parameter_list.parameter_list.empty());

    ASSERT_TRUE(body.statements.empty());
}
//****************************************************************************//
//                              signature_parser                              //
//****************************************************************************//
TEST(TestSignatureParser, NoParameters)
{
    std::array token_stream_raw{
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = signature_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<signature_node>(*get_node(result)));

    auto signature = std::move(std::get<signature_node>(*get_node(result)));

    ASSERT_EQ(signature.identifier, "foo"sv);
    ASSERT_TRUE(
        std::holds_alternative<parameter_def_node>(*(signature.parameter_list)));

    auto parameter_list = std::get<parameter_def_node>(*(signature.parameter_list));
    ASSERT_TRUE(parameter_list.parameter_list.empty());
}
//****************************************************************************//
//                             return_stmt_parser                             //
//****************************************************************************//
TEST(TestReturnStmtParser, Literal)
{
    std::array token_stream_raw{
        token(token_type::RETURN, "return"sv, source_location()),
        token(token_type::LITERAL, "5"sv, source_location()),
        token(token_type::SEMICOLON, ";"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = return_stmt_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<return_stmt_node>(*get_node(result)));

    auto return_stmt = std::move(std::get<return_stmt_node>(*get_node(result)));

    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(return_stmt.value)));

    auto return_value = std::move(std::get<leaf_node>(*(return_stmt.value)));

    ASSERT_EQ(return_value.value, "5"sv);
}

//****************************************************************************//
//                               parameter_def_parser                         //
//****************************************************************************//
TEST(TestParameterDefParser, NoParameter)
{
    std::array token_stream_raw{token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_def_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
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
//                               parameter_pass_parser                        //
//****************************************************************************//
TEST(TestParameterPassParser, NoParameter)
{
    std::array token_stream_raw{token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_pass_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
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
//                               var_decl_parser                              //
//****************************************************************************//
TEST(TestVarDeclParser, Simple)
{
    std::array token_stream_raw{token(token_type::LET, "let"sv, source_location()),
                                token(token_type::IDENTIFIER, "x"sv, source_location()),
                                token(token_type::SEMICOLON, ";"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = var_decl_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);

    ASSERT_TRUE(std::holds_alternative<var_decl_node>((*get_node(result))));
    auto var_decl = std::move(std::get<var_decl_node>((*get_node(result))));

    ASSERT_EQ(var_decl.identifier, "x"sv);
}
//****************************************************************************//
//                            var_assignment_parser                           //
//****************************************************************************//
TEST(TestVarAssignmentParser, Literal)
{
    std::array token_stream_raw{token(token_type::IDENTIFIER, "x"sv, source_location()),
                                token(token_type::ASSIGN, "="sv, source_location()),
                                token(token_type::LITERAL, "5"sv, source_location()),
                                token(token_type::SEMICOLON, ";"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = var_assignment_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);

    ASSERT_TRUE(std::holds_alternative<var_assignment_node>((*get_node(result))));
    auto var_assignment = std::move(std::get<var_assignment_node>((*get_node(result))));

    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(var_assignment.value)));
    auto value = std::move(std::get<leaf_node>(*(var_assignment.value)));

    ASSERT_EQ(value.value, "5"sv);
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
//                                 call_parser                                //
//****************************************************************************//
TEST(TestCallParser, NoParameters)
{
    std::array token_stream_raw{
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = call_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<call_node>(*get_node(result)));

    auto call = std::move(std::get<call_node>(*get_node(result)));

    ASSERT_EQ(call.identifier, "foo"sv);
    ASSERT_TRUE(std::holds_alternative<parameter_pass_node>(*(call.parameter_pass)));

    auto parameter_list = std::get<parameter_pass_node>(*(call.parameter_pass));
    ASSERT_TRUE(parameter_list.parameter_list.empty());
}

//****************************************************************************//
//                               if_stmt_parser                               //
//****************************************************************************//
TEST(TestIfStmtParser, LiteralHeadEmptyBody)
{
    std::array token_stream_raw{token(token_type::IF, "if"sv, source_location()),
                                token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::LITERAL, "1"sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location()),
                                token(token_type::LBRACE, "{"sv, source_location()),
                                token(token_type::RBRACE, "}"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = if_stmt_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);

    ASSERT_TRUE(std::holds_alternative<if_stmt_node>(*get_node(result)));
    auto if_stmt = std::move(std::get<if_stmt_node>(*get_node(result)));

    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(if_stmt.condition)));
    auto condition = std::move(std::get<leaf_node>(*(if_stmt.condition)));
    ASSERT_EQ(condition.value, "1"sv);

    ASSERT_TRUE(std::holds_alternative<block_node>(*(if_stmt.body)));
    auto body = std::move(std::get<block_node>(*(if_stmt.body)));

    ASSERT_TRUE(body.statements.empty());
}
//****************************************************************************//
//                             else_if_stmt_parser                            //
//****************************************************************************//
TEST(TestElseIfStmtParser, LiteralHeadEmptyBody)
{
    std::array token_stream_raw{token(token_type::ELSE, "else"sv, source_location()),
                                token(token_type::IF, "if"sv, source_location()),
                                token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::LITERAL, "1"sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location()),
                                token(token_type::LBRACE, "{"sv, source_location()),
                                token(token_type::RBRACE, "}"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = else_if_stmt_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);

    ASSERT_TRUE(std::holds_alternative<else_if_stmt_node>(*get_node(result)));
    auto else_if_stmt = std::move(std::get<else_if_stmt_node>(*get_node(result)));

    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(else_if_stmt.condition)));
    auto condition = std::move(std::get<leaf_node>(*(else_if_stmt.condition)));
    ASSERT_EQ(condition.value, "1"sv);

    ASSERT_TRUE(std::holds_alternative<block_node>(*(else_if_stmt.body)));
    auto body = std::move(std::get<block_node>(*(else_if_stmt.body)));

    ASSERT_TRUE(body.statements.empty());
}
//****************************************************************************//
//                              else_stmt_parser                              //
//****************************************************************************//
TEST(TestElseStmtParser, EmptyBody)
{
    std::array token_stream_raw{token(token_type::ELSE, "else"sv, source_location()),
                                token(token_type::LBRACE, "{"sv, source_location()),
                                token(token_type::RBRACE, "}"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = else_stmt_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);

    ASSERT_TRUE(std::holds_alternative<else_stmt_node>(*get_node(result)));
    auto else_stmt = std::move(std::get<else_stmt_node>(*get_node(result)));

    ASSERT_TRUE(std::holds_alternative<block_node>(*(else_stmt.body)));
    auto body = std::move(std::get<block_node>(*(else_stmt.body)));

    ASSERT_TRUE(body.statements.empty());
}
//****************************************************************************//
//                               for_loop_parser                              //
//****************************************************************************//
TEST(TestForLoopParser, HeadZeroTo5EmptyBody)
{
    std::array token_stream_raw{token(token_type::FOR, "for"sv, source_location()),
                                token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::LET, "let"sv, source_location()),
                                token(token_type::IDENTIFIER, "i"sv, source_location()),
                                token(token_type::ASSIGN, "="sv, source_location()),
                                token(token_type::LITERAL, "0"sv, source_location()),
                                token(token_type::SEMICOLON, ";"sv, source_location()),
                                token(token_type::IDENTIFIER, "i"sv, source_location()),
                                token(token_type::LESS, "<"sv, source_location()),
                                token(token_type::LITERAL, "5"sv, source_location()),
                                token(token_type::SEMICOLON, ";"sv, source_location()),
                                token(token_type::INCREMENT, "++"sv, source_location()),
                                token(token_type::IDENTIFIER, "i"sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location()),
                                token(token_type::LBRACE, "{"sv, source_location()),
                                token(token_type::RBRACE, "}"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = for_loop_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);

    ASSERT_TRUE(std::holds_alternative<for_loop_node>(*get_node(result)));
    auto for_loop = std::move(std::get<for_loop_node>(*get_node(result)));

    ASSERT_TRUE(std::holds_alternative<var_init_node>(*(for_loop.init_stmt)));
    ASSERT_TRUE(std::holds_alternative<binary_op_node>(*(for_loop.test_expression)));
    ASSERT_TRUE(std::holds_alternative<unary_op_node>(*(for_loop.update_expression)));

    ASSERT_TRUE(std::holds_alternative<block_node>(*(for_loop.body)));
    auto body = std::move(std::get<block_node>(*(for_loop.body)));
    ASSERT_TRUE(body.statements.empty());
}
TEST(TestForLoopParser, EmptyHeadEmptyBody)
{
    std::array token_stream_raw{token(token_type::FOR, "for"sv, source_location()),
                                token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::SEMICOLON, ";"sv, source_location()),
                                token(token_type::SEMICOLON, ";"sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location()),
                                token(token_type::LBRACE, "{"sv, source_location()),
                                token(token_type::RBRACE, "}"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = for_loop_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);

    ASSERT_TRUE(std::holds_alternative<for_loop_node>(*get_node(result)));
    auto for_loop = std::move(std::get<for_loop_node>(*get_node(result)));

    ASSERT_EQ(for_loop.init_stmt, nullptr);
    ASSERT_EQ(for_loop.test_expression, nullptr);
    ASSERT_EQ(for_loop.update_expression, nullptr);

    ASSERT_TRUE(std::holds_alternative<block_node>(*(for_loop.body)));
    auto body = std::move(std::get<block_node>(*(for_loop.body)));
    ASSERT_TRUE(body.statements.empty());
}
//****************************************************************************//
//                              while_loop_parser                             //
//****************************************************************************//
TEST(TestWhileLoopParser, LiteralHeadEmptyBody)
{
    std::array token_stream_raw{token(token_type::WHILE, "while"sv, source_location()),
                                token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::LITERAL, "1"sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location()),
                                token(token_type::LBRACE, "{"sv, source_location()),
                                token(token_type::RBRACE, "}"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = while_loop_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);

    ASSERT_TRUE(std::holds_alternative<while_loop_node>(*get_node(result)));
    auto while_loop = std::move(std::get<while_loop_node>(*get_node(result)));

    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(while_loop.condition)));
    auto condition = std::move(std::get<leaf_node>(*(while_loop.condition)));
    ASSERT_EQ(condition.value, "1"sv);

    ASSERT_TRUE(std::holds_alternative<block_node>(*(while_loop.body)));
    auto body = std::move(std::get<block_node>(*(while_loop.body)));

    ASSERT_TRUE(body.statements.empty());
}
//****************************************************************************//
//                                switch_parser                               //
//****************************************************************************//
TEST(TestSwitchParser, LiteralHeadEmptyBody)
{
    std::array token_stream_raw{
        token(token_type::SWITCH, "switch"sv, source_location()),
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::LITERAL, "1"sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location()),
        token(token_type::LBRACE, "{"sv, source_location()),
        token(token_type::RBRACE, "}"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = switch_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);

    ASSERT_TRUE(std::holds_alternative<switch_node>(*get_node(result)));
    auto switch_stmt = std::move(std::get<switch_node>(*get_node(result)));

    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(switch_stmt.expression)));
    auto condition = std::move(std::get<leaf_node>(*(switch_stmt.expression)));
    ASSERT_EQ(condition.value, "1"sv);

    ASSERT_TRUE(std::holds_alternative<block_node>(*(switch_stmt.body)));
    auto body = std::move(std::get<block_node>(*(switch_stmt.body)));

    ASSERT_TRUE(body.statements.empty());
}
//****************************************************************************//
//                                 case_parser                                //
//****************************************************************************//
TEST(TestCaseParser, LiteralHeadEmptyBody)
{
    std::array token_stream_raw{token(token_type::CASE, "case"sv, source_location()),
                                token(token_type::LITERAL, "1"sv, source_location()),
                                token(token_type::COLON, ":"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = case_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);

    ASSERT_TRUE(std::holds_alternative<case_node>(*get_node(result)));
    auto case_stmt = std::move(std::get<case_node>(*get_node(result)));

    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(case_stmt.value)));
    auto condition = std::move(std::get<leaf_node>(*(case_stmt.value)));
    ASSERT_EQ(condition.value, "1"sv);

    ASSERT_TRUE(std::holds_alternative<block_node>(*(case_stmt.body)));
    auto body = std::move(std::get<block_node>(*(case_stmt.body)));

    ASSERT_TRUE(body.statements.empty());
};
TEST(TestCaseParser, LiteralHeadCallBody)
{
    std::array token_stream_raw{
        token(token_type::CASE, "case"sv, source_location()),
        token(token_type::LITERAL, "1"sv, source_location()),
        token(token_type::COLON, ":"sv, source_location()),
        token(token_type::IDENTIFIER, "foo"sv, source_location()),
        token(token_type::LPAREN, "("sv, source_location()),
        token(token_type::RPAREN, ")"sv, source_location()),
        token(token_type::SEMICOLON, ";"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = case_parser::parse(token_stream);

    ASSERT_TRUE(std::holds_alternative<parse_content>(result));
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);

    ASSERT_TRUE(std::holds_alternative<case_node>(*get_node(result)));
    auto case_stmt = std::move(std::get<case_node>(*get_node(result)));

    ASSERT_TRUE(std::holds_alternative<leaf_node>(*(case_stmt.value)));
    auto condition = std::move(std::get<leaf_node>(*(case_stmt.value)));
    ASSERT_EQ(condition.value, "1"sv);

    ASSERT_TRUE(std::holds_alternative<block_node>(*(case_stmt.body)));
    auto body = std::move(std::get<block_node>(*(case_stmt.body)));

    ASSERT_EQ(body.statements.size(), 1);
    ASSERT_TRUE(std::holds_alternative<call_node>(*(body.statements[0])));
};
