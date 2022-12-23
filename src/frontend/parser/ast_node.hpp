// Copyright © 2021-2022 Jonas Muehlmann
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <variant>
#include <vector>

#include "ast_node_type.hpp"
#include "common/macros.hpp"
#include "common/source_location.hpp"
#include "frontend/lexer/token.hpp"
#include "frontend/parser/parse_error.hpp"
#include "token_type.hpp"

struct program_node;
struct binary_op_node;
struct unary_op_node;
struct func_def_node;
struct procedure_def_node;
struct signature_node;
struct return_stmt_node;
struct parameter_def_node;
struct var_decl_node;
struct var_init_node;
struct var_assignment_node;
struct call_node;
struct parameter_pass_node;
struct block_node;
struct if_stmt_node;
struct else_if_stmt_node;
struct else_stmt_node;
struct for_loop_node;
struct while_loop_node;
struct switch_node;
struct case_node;
struct missing_optional_node;
struct leaf_node;

using json = nlohmann::ordered_json;

using ast_node_t = std::variant<program_node,
                                binary_op_node,
                                unary_op_node,
                                func_def_node,
                                procedure_def_node,
                                signature_node,
                                return_stmt_node,
                                parameter_def_node,
                                var_decl_node,
                                var_init_node,
                                var_assignment_node,
                                call_node,
                                parameter_pass_node,
                                block_node,
                                if_stmt_node,
                                else_if_stmt_node,
                                else_stmt_node,
                                for_loop_node,
                                while_loop_node,
                                switch_node,
                                case_node,
                                missing_optional_node,
                                leaf_node>;

struct dummy_iterator
{
    using iterator_category = std::random_access_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = ast_node_t;
    using pointer           = ast_node_t*;
    using reference         = ast_node_t&;

    pointer m_ptr;

    // dummy_iterator(pointer ptr) : m_ptr(ptr) {}

    reference operator*() const;

    pointer operator->();

    dummy_iterator& operator++();

    dummy_iterator operator++(int);

    dummy_iterator& operator--();

    dummy_iterator operator--(int);

    difference_type operator-(dummy_iterator rhs);

    // difference_type operator+(dummy_iterator rhs);

    dummy_iterator operator+(std::size_t n);

    dummy_iterator operator-(std::size_t n);

    dummy_iterator& operator+=(std::size_t n);

    dummy_iterator& operator-=(std::size_t n);

    friend bool operator==(const dummy_iterator& a, const dummy_iterator& b);

    friend bool operator!=(const dummy_iterator& a, const dummy_iterator& b);

    dummy_iterator begin() const;
    dummy_iterator end() const;
};

struct ast_node
{
    using iterator = dummy_iterator;

    // TODO: Is  this parameter any useful or just redundant with the struct's types?
    ast_node_type               type;
    source_location             source_location_;
    std::shared_ptr<ast_node_t> parent;

    ast_node(ast_node_type type, source_location location);
    virtual ~ast_node() = default;

    iterator begin() const;
    iterator end() const;
};

struct missing_optional_node
{
    using iterator = dummy_iterator;

    parse_error encountered_error;

    explicit missing_optional_node(parse_error encountered_error);

    iterator begin() const;
    iterator end() const;
};

struct leaf_node final : public ast_node
{
    using iterator = dummy_iterator;

    token_type token;

    std::string_view value;

    leaf_node(token_type token, std::string_view value, source_location location);
    explicit leaf_node(struct token token_);

    ~leaf_node();

    iterator begin() const;
    iterator end() const;
};


struct binary_op_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 3>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> lhs;
            std::shared_ptr<ast_node_t> rhs;
            std::shared_ptr<ast_node_t> operator_;
        };
    };

    binary_op_node(std::shared_ptr<ast_node_t> lhs_,
                   std::shared_ptr<ast_node_t> rhs_,
                   std::shared_ptr<ast_node_t> operator_,
                   source_location             location);
    ~binary_op_node();

    iterator begin() const;
    iterator end() const;
};


struct unary_op_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 2>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> operand;
            std::shared_ptr<ast_node_t> operator_;
        };
    };


    unary_op_node(std::shared_ptr<ast_node_t> operand,
                  std::shared_ptr<ast_node_t> operator_,
                  source_location             location);
    ~unary_op_node();

    iterator begin() const;
    iterator end() const;
};


struct func_def_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 2>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> signature;
            std::shared_ptr<ast_node_t> body;
        };
    };

    func_def_node(std::shared_ptr<ast_node_t> signature,
                  std::shared_ptr<ast_node_t> body,
                  source_location             location);
    ~func_def_node();

    iterator begin() const;
    iterator end() const;
};


struct procedure_def_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 2>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> signature;
            std::shared_ptr<ast_node_t> body;
        };
    };

    procedure_def_node(std::shared_ptr<ast_node_t> signature,
                       std::shared_ptr<ast_node_t> body,
                       source_location             location);
    ~procedure_def_node();

    iterator begin() const;
    iterator end() const;
};


struct signature_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 2>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::string_view            identifier;
            std::shared_ptr<ast_node_t> parameter_list;
        };
    };

    signature_node(std::string_view            identifier,
                   std::shared_ptr<ast_node_t> parameter_list,
                   source_location             location);
    ~signature_node();

    iterator begin() const;
    iterator end() const;
};


struct return_stmt_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 1>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> value;
        };
    };

    return_stmt_node(std::shared_ptr<ast_node_t> value, source_location location);
    ~return_stmt_node();

    iterator begin() const;
    iterator end() const;
};


struct parameter_def_node final : public ast_node
{
    using iterator = dummy_iterator;

    std::vector<std::string_view> parameter_list;

    parameter_def_node(std::vector<std::string_view>&& parameter_list, source_location location);
    ~parameter_def_node();

    iterator begin() const;
    iterator end() const;
};


struct var_decl_node final : public ast_node
{
    using iterator = dummy_iterator;

    std::string_view identifier;

    var_decl_node(std::string_view identifier, source_location location);
    ~var_decl_node();

    iterator begin() const;
    iterator end() const;
};


struct var_init_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 1>;
    using iterator      = union_array_t::iterator;

    std::string_view identifier;
    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> value;
        };
    };

    var_init_node(std::string_view            identifier,
                  std::shared_ptr<ast_node_t> value,
                  source_location             location);
    ~var_init_node();

    iterator begin() const;
    iterator end() const;
};


struct var_assignment_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 1>;
    using iterator      = union_array_t::iterator;

    std::string_view identifier;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> value;
        };
    };

    var_assignment_node(std::string_view            identifier,
                        std::shared_ptr<ast_node_t> value,
                        source_location             location);
    ~var_assignment_node();

    iterator begin() const;
    iterator end() const;
};


struct call_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 1>;
    using iterator      = union_array_t::iterator;

    std::string_view identifier;
    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> parameter_pass;
        };
    };

    call_node(std::string_view            identifier,
              std::shared_ptr<ast_node_t> parameter_pass,
              source_location             location);
    ~call_node();

    iterator begin() const;
    iterator end() const;
};


struct parameter_pass_node final : public ast_node
{
    using iterator = dummy_iterator;

    std::vector<std::string_view> parameter_list;

    parameter_pass_node(std::vector<std::string_view>&& parameter_list, source_location location);
    ~parameter_pass_node();

    iterator begin() const;
    iterator end() const;
};

struct if_stmt_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 2>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> condition;
            std::shared_ptr<ast_node_t> body;
        };
    };

    if_stmt_node(std::shared_ptr<ast_node_t> condition,
                 std::shared_ptr<ast_node_t> body,
                 source_location             location);
    ~if_stmt_node();

    iterator begin() const;
    iterator end() const;
};

struct else_if_stmt_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 2>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> condition;
            std::shared_ptr<ast_node_t> body;
        };
    };

    else_if_stmt_node(std::shared_ptr<ast_node_t> condition,
                      std::shared_ptr<ast_node_t> body,
                      source_location             location);
    ~else_if_stmt_node();

    iterator begin() const;
    iterator end() const;
};

struct else_stmt_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 1>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> body;
        };
    };

    else_stmt_node(std::shared_ptr<ast_node_t> body, source_location location);
    ~else_stmt_node();

    iterator begin() const;
    iterator end() const;
};

struct for_loop_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 4>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> init_stmt;
            std::shared_ptr<ast_node_t> test_expression;
            std::shared_ptr<ast_node_t> update_expression;

            std::shared_ptr<ast_node_t> body;
        };
    };

    for_loop_node(std::shared_ptr<ast_node_t> init_stmt,
                  std::shared_ptr<ast_node_t> test_expression,
                  std::shared_ptr<ast_node_t> update_expression,
                  std::shared_ptr<ast_node_t> body,
                  source_location             location);
    ~for_loop_node();

    iterator begin() const;
    iterator end() const;
};

struct while_loop_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 2>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> condition;
            std::shared_ptr<ast_node_t> body;
        };
    };

    while_loop_node(std::shared_ptr<ast_node_t> condition,
                    std::shared_ptr<ast_node_t> body,
                    source_location             location);
    ~while_loop_node();

    iterator begin() const;
    iterator end() const;
};

struct switch_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 2>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> expression;
            std::shared_ptr<ast_node_t> body;
        };
    };

    switch_node(std::shared_ptr<ast_node_t> expression,
                std::shared_ptr<ast_node_t> body,
                source_location             location);
    ~switch_node();

    iterator begin() const;
    iterator end() const;
};

struct case_node final : public ast_node
{
    using union_array_t = std::array<std::shared_ptr<ast_node_t>, 2>;
    using iterator      = union_array_t::iterator;

    union
    {
        union_array_t data;
        struct
        {
            std::shared_ptr<ast_node_t> value;
            std::shared_ptr<ast_node_t> body;
        };
    };

    case_node(std::shared_ptr<ast_node_t> value,
              std::shared_ptr<ast_node_t> body,
              source_location             location);
    ~case_node();

    iterator begin() const;
    iterator end() const;
};


struct program_node final : public ast_node
{
    using iterator = std::vector<std::shared_ptr<ast_node_t>>::iterator;

    std::vector<std::shared_ptr<ast_node_t>> globals;

    program_node(std::vector<std::shared_ptr<ast_node_t>>&& globals, source_location location);
    ~program_node();

    iterator begin() const;
    iterator end() const;
};

struct block_node final : public ast_node
{
    using iterator = std::vector<std::shared_ptr<ast_node_t>>::iterator;

    std::vector<std::shared_ptr<ast_node_t>> statements;

    block_node(std::vector<std::shared_ptr<ast_node_t>>&& statements, source_location location);
    ~block_node();

    iterator begin() const;
    iterator end() const;
};


// Needed forawrd declaration
void to_json(json& j, const ast_node_t& node);

// Needed forawrd declaration
void to_json(json& j, const leaf_node& node);

// Needed dummy methods because  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED also
// generates json->T conversion functions
void from_json(const json& j, ast_node_t& node);

template <typename T>
inline void to_json(json& j, const std::shared_ptr<T>& node)
{
    if (node != nullptr)
    {
        j = *node;
    }
    else
    {
        j = nullptr;
    }
}

// Needed dummy methods because  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED also
// generates json->T conversion functions
template <typename T>
void from_json(const json& j, std::shared_ptr<T>& node);

// These must be declared after all structs are fully defined, because they are all part
// of ast_node_t
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(program_node, type, source_location_, globals);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(ast_node, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(leaf_node, token, value);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(
    binary_op_node, type, source_location_, lhs, rhs, operator_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(
    unary_op_node, type, source_location_, operand, operator_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(
    func_def_node, type, source_location_, signature, body);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(
    procedure_def_node, type, source_location_, signature, body);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(
    signature_node, type, source_location_, identifier, parameter_list);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(return_stmt_node, type, source_location_, value);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(parameter_def_node,
                                             type,
                                             source_location_,
                                             parameter_list);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(var_decl_node, type, source_location_, identifier);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(
    var_init_node, type, source_location_, identifier, value);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(
    var_assignment_node, type, source_location_, identifier, value);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(
    call_node, type, source_location_, identifier, parameter_pass);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(parameter_pass_node,
                                             type,
                                             source_location_,
                                             parameter_list);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(block_node, type, source_location_, statements);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(if_stmt_node, type, condition, body, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(
    else_if_stmt_node, type, condition, body, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(else_stmt_node, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(
    for_loop_node, type, init_stmt, test_expression, update_expression, body, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(
    while_loop_node, type, condition, body, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(switch_node, type, expression, body, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_UNORDERED(case_node, type, value, body, source_location_);

inline void to_json(json& j, const ast_node_t& node)
{
    // std::visit([&j](const auto& node) { to_json(j, node); }, node);
    std::visit([&](auto&& value) { j = std::forward<decltype(value)>(value); }, node);
}
