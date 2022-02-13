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

#include <forward_list>
#include <memory>
#include <variant>

#include "ast_node.hpp"

struct node_lister_visitor
{
    std::forward_list<std::shared_ptr<ast_node_t>>& results;

    explicit node_lister_visitor(
        std::forward_list<std::shared_ptr<ast_node_t>>& results_out) :
        results{results_out}
    {}

    void operator()(std::shared_ptr<program_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        for (auto& statement : node->globals)
        {
            std::visit(node_lister_visitor(results), statement);
        }
    }
    void operator()(std::shared_ptr<binary_op_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node->lhs);
        std::visit(node_lister_visitor(results), node->rhs);
    }
    void operator()(std::shared_ptr<unary_op_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node->operand);
    }
    void operator()(std::shared_ptr<func_def_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node->signature);
        std::visit(node_lister_visitor(results), node.body);
    }
    void operator()(std::shared_ptr<procedure_def_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node.signature);
        std::visit(node_lister_visitor(results), node.body);
    }
    void operator()(std::shared_ptr<signature_node> node)
    {
        std::visit(node_lister_visitor(results), node);
    }
    void operator()(std::shared_ptr<return_stmt_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node.value);
    }
    void operator()(std::shared_ptr<parameter_def_node> node)
    {
        std::visit(node_lister_visitor(results), node);
    }
    void operator()(std::shared_ptr<var_decl_node> node)
    {
        std::visit(node_lister_visitor(results), node);
    }
    void operator()(std::shared_ptr<var_init_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node.value);
    }
    void operator()(std::shared_ptr<var_assignment_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node.value);
    }
    void operator()(std::shared_ptr<call_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node.parameter_pass);
    }
    void operator()(std::shared_ptr<parameter_pass_node> node)
    {
        std::visit(node_lister_visitor(results), node);
    }
    void operator()(std::shared_ptr<block_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        for (auto& statement : node.statements)
        {
            std::visit(node_lister_visitor(results), statement);
        }
    }
    void operator()(std::shared_ptr<if_stmt_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node.condition);
        std::visit(node_lister_visitor(results), node.body);
    }
    void operator()(std::shared_ptr<else_if_stmt_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node.condition);
        std::visit(node_lister_visitor(results), node.body);
    }
    void operator()(std::shared_ptr<else_stmt_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node.body);
    }
    void operator()(std::shared_ptr<for_loop_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node.init_stmt);
        std::visit(node_lister_visitor(results), node.test_expression);
        std::visit(node_lister_visitor(results), node.update_expression);
        std::visit(node_lister_visitor(results), node.body);
    }
    void operator()(std::shared_ptr<while_loop_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node.condition);
        std::visit(node_lister_visitor(results), node.body);
    }
    void operator()(std::shared_ptr<switch_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node.expression);
        std::visit(node_lister_visitor(results), node.body);
    }
    void operator()(std::shared_ptr<case_node> node)
    {
        std::visit(node_lister_visitor(results), node);
        std::visit(node_lister_visitor(results), node.value);
        std::visit(node_lister_visitor(results), node.body);
    }
    void operator()(std::shared_ptr<missing_optional_node> node)
    {
        ;
    }
    void operator()(std::shared_ptr<leaf_node> node)
    {
        std::visit(node_lister_visitor(results), node);
    }
};
