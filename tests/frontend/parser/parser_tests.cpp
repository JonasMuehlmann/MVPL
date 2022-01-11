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

    ASSERT_TRUE(result.has_value());
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

    ASSERT_TRUE(result.has_value());
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

    ASSERT_TRUE(result.has_value());
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

    ASSERT_TRUE(result.has_value());
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

    ASSERT_TRUE(result.has_value());
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

    ASSERT_FALSE(result.has_value());
}

//****************************************************************************//
//                               ParameterPassParser                              //
//****************************************************************************//
TEST(TestParameterPassParser, NoParameter)
{
    std::array token_stream_raw{token(token_type::LPAREN, "("sv, source_location()),
                                token(token_type::RPAREN, ")"sv, source_location())};

    std::span<token> token_stream(token_stream_raw);

    auto result = parameter_pass_parser::parse(token_stream);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_EQ(std::get<parameter_def_node>((*get_node(result))).parameter_list.size(),
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

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<parameter_def_node>((*get_node(result))));
    ASSERT_EQ(std::get<parameter_def_node>((*get_node(result))).parameter_list.size(),
              1);
    ASSERT_EQ(std::get<parameter_def_node>((*get_node(result))).parameter_list[0],
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

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<parameter_def_node>((*get_node(result))));
    ASSERT_EQ(std::get<parameter_def_node>((*get_node(result))).parameter_list.size(),
              2);
    ASSERT_TRUE(std::ranges::equal(
        std::get<parameter_def_node>((*get_node(result))).parameter_list,
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

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<parameter_def_node>((*get_node(result))));
    ASSERT_EQ(std::get<parameter_def_node>((*get_node(result))).parameter_list.size(),
              3);
    ASSERT_TRUE(std::ranges::equal(
        std::get<parameter_def_node>((*get_node(result))).parameter_list,
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

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(get_token_stream(result).size(), 0);
    ASSERT_NE(get_node(result), nullptr);
    ASSERT_TRUE(std::holds_alternative<parameter_def_node>((*get_node(result))));
    ASSERT_EQ(std::get<parameter_def_node>((*get_node(result))).parameter_list.size(),
              4);
    ASSERT_TRUE(std::ranges::equal(
        std::get<parameter_def_node>((*get_node(result))).parameter_list,
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

    ASSERT_FALSE(result.has_value());
}
