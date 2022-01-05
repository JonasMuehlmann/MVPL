// Copyright © 2021 Jonas Muehlmann
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
#ifndef SRC_FRONTEND_PARSER_AST_NODE_HPP_
#define SRC_FRONTEND_PARSER_AST_NODE_HPP_

#include <memory>
#include <variant>
#include <vector>

#include "ast_node_type.hpp"
#include "common/source_location.hpp"
#include "frontend/lexer/token.hpp"
#include "nlohmann/json.hpp"
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
struct control_block_node;
struct control_head_node;
struct empty_node;
struct leaf_node;

using json = nlohmann::json;

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
                                control_block_node,
                                control_head_node,
                                empty_node,
                                leaf_node>;


struct ast_node
{
    // TODO: Is  this parameter any useful or just redundant with the struct's types?
    ast_node_type   type;
    source_location source_location_;

    ast_node(ast_node_type type, source_location location);
    virtual ~ast_node() = default;
};

struct empty_node
{};

struct leaf_node final : public ast_node
{
    token_type token;

    std::string_view value;

    leaf_node(token_type token, std::string_view value, source_location location);
    // TODO:  add token based constructor
    // leaf_node(token token_);
};

struct program_node final : public ast_node
{
    std::vector<std::unique_ptr<ast_node_t>>& globals;

    program_node(std::vector<std::unique_ptr<ast_node_t>>& globals,
                 source_location                           location);
};

struct binary_op_node final : public ast_node
{
    std::unique_ptr<ast_node_t> lhs;
    std::unique_ptr<ast_node_t> rhs;
    std::unique_ptr<ast_node_t> operator_;

    binary_op_node(std::unique_ptr<ast_node_t>& lhs_,
                   std::unique_ptr<ast_node_t>& rhs_,

                   std::unique_ptr<ast_node_t>& operator_,
                   source_location              location);
};


struct unary_op_node final : public ast_node
{
    std::unique_ptr<ast_node_t> operand;
    std::unique_ptr<ast_node_t> operator_;

    unary_op_node(std::unique_ptr<ast_node_t>& operand,

                  std::unique_ptr<ast_node_t>& operator_,
                  source_location              location);
};


struct func_def_node final : public ast_node
{
    std::unique_ptr<ast_node_t> signature;
    std::unique_ptr<ast_node_t> body;

    func_def_node(std::unique_ptr<ast_node_t>& signature,
                  std::unique_ptr<ast_node_t>& body,
                  source_location              location);
};


struct procedure_def_node final : public ast_node
{
    std::unique_ptr<ast_node_t> signature;
    std::unique_ptr<ast_node_t> body;

    procedure_def_node(std::unique_ptr<ast_node_t>& signature,
                       std::unique_ptr<ast_node_t>& body,
                       source_location              location);
};


struct signature_node final : public ast_node
{
    std::string&                identifier;
    std::unique_ptr<ast_node_t> parameter_list;

    signature_node(std::string&                 identifier,
                   std::unique_ptr<ast_node_t>& parameter_list,
                   source_location              location);
};


struct return_stmt_node final : public ast_node
{
    std::unique_ptr<ast_node_t> value;

    return_stmt_node(std::unique_ptr<ast_node_t>& value, source_location location);
};


struct parameter_def_node final : public ast_node
{
    std::unique_ptr<ast_node_t> parameter_list;

    parameter_def_node(std::unique_ptr<ast_node_t>& parameter_list,
                       source_location              location);
};


struct var_decl_node final : public ast_node
{
    std::string& identifier;

    var_decl_node(std::string& identifier, source_location location);
};


struct var_init_node final : public ast_node
{
    std::string&                identifier;
    std::unique_ptr<ast_node_t> value;

    var_init_node(std::string&                 identifier,
                  std::unique_ptr<ast_node_t>& value,
                  source_location              location);
};


struct var_assignment_node final : public ast_node
{
    std::string&                identifier;
    std::unique_ptr<ast_node_t> value;

    var_assignment_node(std::string&                 identifier,
                        std::unique_ptr<ast_node_t>& value,
                        source_location              location);
};


struct call_node final : public ast_node
{
    std::string&                identifier;
    std::unique_ptr<ast_node_t> parameter_pass;

    call_node(std::string&                 identifier,
              std::unique_ptr<ast_node_t>& parameter_pass,
              source_location              location);
};


struct parameter_pass_node final : public ast_node
{
    std::unique_ptr<ast_node_t> parameter_list;

    parameter_pass_node(std::unique_ptr<ast_node_t>& parameter_list,
                        source_location              location);
};


struct block_node final : public ast_node
{
    std::vector<std::unique_ptr<ast_node_t>> statements;

    block_node(std::vector<std::unique_ptr<ast_node_t>>& statements,
               source_location                           location);
};


struct control_block_node final : public ast_node
{
    std::unique_ptr<ast_node_t> head;
    std::unique_ptr<ast_node_t> body;

    control_block_node(std::unique_ptr<ast_node_t>& head,
                       std::unique_ptr<ast_node_t>& body,
                       source_location              location);
};


struct control_head_node final : public ast_node
{
    std::unique_ptr<ast_node_t> expression;

    control_head_node(std::unique_ptr<ast_node_t>& expression,
                      source_location              location);
};

// Needed forawrd declaration
void to_json(json& j, const ast_node_t& node);

// Needed forawrd declaration
void to_json(json& j, const leaf_node& node);

// Needed dummy methods because  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE also generates
// json->T conversion functions
void from_json(const json& j, ast_node_t& node);

template <typename T>
inline void to_json(json& j, const std::unique_ptr<T>& node)
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

// Needed dummy methods because  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE also generates
// json->T conversion functions
template <typename T>
void from_json(const json& j, std::unique_ptr<T>& node);

// These must be declared after all structs are fully defined, because they are all part
// of ast_node_t
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(program_node, globals, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ast_node, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(leaf_node, token, value);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    binary_op_node, lhs, rhs, operator_, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    unary_op_node, operand, operator_, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    func_def_node, signature, body, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    procedure_def_node, signature, body, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    signature_node, identifier, parameter_list, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(return_stmt_node, value, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(parameter_def_node,
                                   parameter_list,
                                   type,
                                   source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(var_decl_node, identifier, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    var_init_node, identifier, value, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    var_assignment_node, identifier, value, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    call_node, identifier, parameter_pass, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(parameter_pass_node,
                                   parameter_list,
                                   type,
                                   source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(block_node, statements, type, source_location_);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    control_block_node, head, body, type, source_location_);

inline void to_json(json& j, const ast_node_t& node)
{
    // std::visit([&j](const auto& node) { to_json(j, node); }, node);
    std::visit([&](auto&& value) { j = std::forward<decltype(value)>(value); }, node);
}
#endif    // SRC_FRONTEND_PARSER_AST_NODE_HPP_
